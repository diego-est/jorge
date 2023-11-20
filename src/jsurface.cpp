#include <LSceneView.h>
#include <LCursor.h>
#include "jsurface.hpp"
#include "global.hpp"
#include "jorge.hpp"
#include "joatbar_item.hpp"
#include "joutput.hpp"
#include "data.hpp"

JSurface::JSurface(data::ptr<LSurface::Params> params)
	: LSurface(params)
	, view(this, &J::compositor()->surfacesLayer)
{
	view.enableParentOffset(false);
}

JSurface::~JSurface()
{
	/* destroy thumbnails */
	while (!topbarItems.empty())
		delete topbarItems.back();

	if (thumbnail)
		delete thumbnail;
}

auto JSurface::roleChanged() -> void
{
	/* hide cursor surfaces because we use LCursor */
	if (cursorRole()) {
		view.setVisible(false);
	} else if (dndIcon()) {
	/* Move drag-n-drop icons to the cursor layer so they always appear on
	* top of other views
	*/
		view.setParent(&J::compositor()->cursorLayer);
		view.insertAfter(nullptr, false);
		setPos(cursor()->pos());
	}
}

auto JSurface::orderChanged() -> void
{
	auto prev = (data::ptr<JSurface>)prevSurface();

	/* if prev has a different parent view, keep searching for a match */
	while (prev && prev->view.parent() != view.parent())
		prev = (data::ptr<JSurface>)prev->prevSurface();

	/* if prev has the same parent view, we insert it */
	if (prev)
		view.insertAfter(&prev->view, false);
	else /* if there is no prev surface, insert it at the beginning of the
		* current parent's children list
		*/
		view.insertAfter(nullptr, false);
}

auto JSurface::minimizedChanged() -> void
{
	/* remove pointer and keyboard focus */
	if (minimized()) {
		if (hasPointerFocus())
			seat()->pointer()->setFocus(nullptr);

		if (hasKeyboardFocus())
			seat()->keyboard()->setFocus(nullptr);
	}

	if (!toplevel()) {
		view.setVisible(!minimized());
		return;
	}

	if (minimized()) {
		minimizedOutput = primaryOutput();

		if (minimizedOutput) {
			/* save the current surface position relative to the
			* output position as a percentage so we can restore it
			* later even if the outputs arrangement changes or the
			* given output is no longer available
			*/
			auto localPos = rolePos() - minimizedOutput->pos();
			prevMinimizedPos = localPos / LSizeF(minimizedOutput->size());
		} else {
			/* in case the surface is not visible on any output, we
			* select the first available output and position the
			* surface at 1/4 of the output as fallback
			*/
			minimizedOutput = J::outputs().front();
			prevMinimizedPos = LPointF(0.25f, 0.25f);
		}

		LSceneView tmpScene(sizeB(), bufferScale());
		capture(&tmpScene);

		/* scale it to thumbnail size */
		thumbnail = tmpScene.texture()->copyB(
			LSize((THUMBNAIL_HEIGHT * sizeB().w()) / sizeB().h(), THUMBNAIL_HEIGHT) * 2
		);

		/* create a topbar item for each topbar */
		for (auto output : J::outputs())
			new JoatbarItem(output->topbar, this);

		view.setVisible(false);
	} else {
		/* if minimized output is nullptr, it's because it was
		 * uninitialized while the surface was minimized
		 */
		if (!minimizedOutput) {
			minimizedOutput = J::outputs().front();
			prevMinimizedPos = LPointF(0.25f, 0.25f);
		}

		/* destroy thumbnails */
		while (!topbarItems.empty())
			delete topbarItems.front();

		delete thumbnail;
		thumbnail = nullptr;

		/* restore back the previous unminimized position */
		setPos(minimizedOutput->pos() + (prevMinimizedPos * minimizedOutput->size()));
		minimizedOutput = nullptr;
		view.setVisible(true);

		/* stack the surface above the rest */
		raise();
	}
}

/* returns the output where the surface is mainly visible or nullptr if not
* visible in any output
*/
auto JSurface::primaryOutput() -> data::ptr<JOutput>
{
	auto bestOutput = (data::ptr<JOutput>)nullptr;
	auto bestArea = 0u;
	auto surfaceRect = (LRect)0;

	/* ignore the decoration of toplevel and popup roles */
	if (toplevel())
		surfaceRect = LRect(rolePos() + toplevel()->windowGeometry().pos(), toplevel()->windowGeometry().size());
	else if (popup())
		surfaceRect = LRect(rolePos() + popup()->windowGeometry().pos(), popup()->windowGeometry().size());
	else
		surfaceRect = LRect(rolePos(), size());

	/* calculate the area of the surface intersected with each output and
	* return the one with the largest area
	*/
	for (auto output : J::outputs()) {
		/* we use LRegion to intersect both rects */
		auto tmpRegion = (LRegion)0;
		tmpRegion.addRect(surfaceRect);
		tmpRegion.clip(output->rect());

		auto extents = tmpRegion.extents();
		auto area = (unsigned)(extents.x2 - extents.x1) * (extents.y2 - extents.y1);

		if (area > bestArea) {
			bestArea = area;
			bestOutput = output;
		}
	}

	return bestOutput;
}

auto JSurface::capture(data::ptr<LSceneView> sceneView) -> void
{
	/* Instead of moving each scene to the scene view, we move the scene to
	* to the views position. This is why disabling the parent offset is
	* required
	*/
	sceneView->setPos(rolePos());

	/* Add the view and any child subsurfaces to the scene. Notice that we
	* exclude child surfaces with the popup or toplevel role.
	*/
	J::moveSurfaceWithChildren(this, sceneView, true);
	sceneView->render();

	/* Restore views to the surfaces layer */
	while (!sceneView->children().empty())
		sceneView->children().front()->setParent(&J::compositor()->surfacesLayer);
}
