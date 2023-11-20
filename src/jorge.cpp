#include <LOutput.h>
#include <LSceneView.h>
#include <memory>
#include "jorge.hpp"
#include "jsurface.hpp"
#include "joutput.hpp"
#include "data.hpp"

JCompositor::JCompositor()
	: LCompositor()
	, scene()
	, backgroundLayer(scene.mainView())
	, surfacesLayer(scene.mainView())
	, overlayLayer(scene.mainView())
	, fullscreenLayer(scene.mainView())
	, cursorLayer(scene.mainView())
	, softwareCursor(nullptr, &cursorLayer)
{
	/* set black as the scene clear color */
	scene.mainView()->setClearColor(0.0, 0.0, 0.0, 1.0);

	/* this allows us to define a custom size to an LTextureView */
	softwareCursor.enableDstSize(true);
}

auto JCompositor::initialized() -> void
{
	/* Set a keyboard with "latam" layout */
	seat()->keyboard()->setKeymap(nullptr, nullptr, "us", "dvorak");

	auto totalWidth = 0;

	/* Initialize all available outputs */
	for (auto output : seat()->outputs()) {
		/* Set double-scale for outputs with DPI >= 200 */
		output->setScale(output->dpi() >= 200 ? 2 : 1);

		output->setPos(LPoint(totalWidth, 0));
		totalWidth += output->size().w();

		addOutput(output);
		output->repaint();

	}
}

auto JCompositor::createOutputRequest() -> data::ptr<LOutput>
{
	return new JOutput();
}

auto JCompositor::createSurfaceRequest(data::ptr<LSurface::Params> params) -> data::ptr<LSurface>
{
	return new JSurface(params);
}
