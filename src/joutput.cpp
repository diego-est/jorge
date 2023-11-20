#include <LPainter.h>
#include <LTexture.h>
#include <LOpenGL.h>
#include <LLog.h>
#include <LDNDManager.h>
#include <LCompositor.h>
#include <LCursor.h>
#include <LSeat.h>
#include <LTextureView.h>
#include "jorge.hpp"
#include "joutput.hpp"
#include "joatbar.hpp"
#include "jsurface.hpp"
#include "global.hpp"
#include "data.hpp"

JOutput::JOutput()
{
}

auto JOutput::initializeGL() -> void
{
	wallpaperView = new LTextureView(nullptr, &J::compositor()->backgroundLayer);
	/* TextureViews have a default nullptr transucent region, which means
	* they are considered completely translucent by the scene. However,
	* setting an empty LRegion (not nullptr) indicates to the scene that the
	* view si completly opaque. */
	auto emptyRegion = LRegion();
	wallpaperView->setTranslucentRegion(&emptyRegion);

	updateWallpaper();

	topbar = new Joatbar(this);
	J::scene()->handleInitializeGL(this);
	repaint();
}

auto JOutput::moveGL() -> void
{
	wallpaperView->setPos(pos());
	topbar->update();
	J::scene()->handleMoveGL(this);
	repaint();
}

auto JOutput::resizeGL() -> void
{
	updateWallpaper();
	topbar->update();
	J::scene()->handleResizeGL(this);
	repaint();
}

auto JOutput::paintGL() -> void
{
	if (cursor()->hasHardwareSupport(this)) {
		J::compositor()->softwareCursor.setTexture(nullptr);
	} else {
		J::compositor()->softwareCursor.setTexture(cursor()->texture());
		J::compositor()->softwareCursor.setPos(cursor()->rect().pos());
		J::compositor()->softwareCursor.setDstSize(cursor()->rect().size());
		J::compositor()->softwareCursor.setVisible(cursor()->visible());
	}

	J::scene()->handlePaintGL(this);
	/* uncomment below and comment above to test damage surfaces
	painter()->clearScreen();
	J::scene()->handlePaintGL(this);

	auto damage = LRegion();
	damage.addRect(rect());
	setBufferDamage(damage);
	*/
}

auto JOutput::uninitializeGL() -> void
{
	/* set minimize output to nullptr to prevent the compositor from
	 * crashing when a surface is unminimized and
	 * JSurface::minimizedChanged() is triggered
	 */
	for (auto surface : J::surfaces())
		if (surface->minimizedOutput == this)
			surface->minimizedOutput = nullptr;
	J::scene()->handlePaintGL(this);

	if (wallpaperView->texture())
		delete wallpaperView->texture();
}

auto JOutput::updateWallpaper() -> void
{
	if (wallpaperView->texture()) {
		/* if the current wallpaper size is equal to the current output size
		* then the output simply changed its scale
		*/
		if (wallpaperView->texture()->sizeB() == sizeB()) {
			wallpaperView->setBufferScale(scale());
			wallpaperView->setPos(pos());
			return;
		}

		delete wallpaperView->texture();
	}

	/* load the original wallpaper */
	auto originalWallpaper = LOpenGL::loadTexture("/home/Michorron/.config/jorge/wallpaper");

	if (!originalWallpaper) {
		LLog::error("[louvre-example] failed to load wallpaper");
		return;
	}

	/* This section calculates the wallpaper rect that needs to be copied
	 * and scaled to cover the entire output while maintaining the image's
	 * aspect ratio.
	*/

	/* define the source rect within the texture to clip */
	auto srcRect = LRect(0);

	/* calculate the texture's width if it were scaled to the output's
	* height, while preserving the aspect ratio
	*/
	auto scaledWidth = (float)(size().w() * originalWallpaper->sizeB().h()) / (float)(size().h());

	/* if the scaled with is greater than or equal to the output width, we
	* clip the texture's left and right sides
	*/
	if (scaledWidth >= originalWallpaper->sizeB().w()) {
		srcRect.setW(originalWallpaper->sizeB().w());
		srcRect.setH((originalWallpaper->sizeB().w() * size().h()) / size().w());
		srcRect.setY((originalWallpaper->sizeB().h() - srcRect.h()) / 2);
	} else {
		/* otherwise clip texture's top and bottom */
		srcRect.setH(originalWallpaper->sizeB().h());
		srcRect.setW((originalWallpaper->sizeB().h() * size().w()) / size().h());
		srcRect.setX((originalWallpaper->sizeB().w() - srcRect.w()) / 2);
	}

	/* copy the srcRect of the original wallpaper and scale it to match the
	* output buffer size
	*/
	wallpaperView->setTexture(originalWallpaper->copyB(sizeB(), srcRect));

	/* set the buffer scale of wallpaperView to match the output scale */
	wallpaperView->setBufferScale(scale());

	/* delete the original wallpaper since we are using the scaled copy */
	delete originalWallpaper;
	wallpaperView->setPos(pos());

	if (topbar)
		delete topbar;
	topbar = nullptr;
}
