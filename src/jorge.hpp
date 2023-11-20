#pragma once
#include <LCompositor.h>
#include <LScene.h>
#include <LLayerView.h>
#include <LTextureView.h>
#include "data.hpp"


using namespace Louvre;

class JCompositor : public LCompositor {
      public:
	JCompositor();

	auto initialized() -> void override;

	/* Virtual constructors */
	auto createOutputRequest() -> data::ptr<LOutput> override;
	auto createSurfaceRequest(data::ptr<LSurface::Params> params) -> data::ptr<LSurface> override;

	/* Scene and Layers */
	LScene scene;
	LLayerView backgroundLayer;
	LLayerView surfacesLayer;
	LLayerView overlayLayer;
	LLayerView fullscreenLayer;
	LLayerView cursorLayer;

	LTextureView softwareCursor;
};

