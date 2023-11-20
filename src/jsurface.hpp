#pragma once
#include <LSurface.h>
#include <LSurfaceView.h>
#include "data.hpp"

using namespace Louvre;

class JoatbarItem;
class JOutput;

class JSurface : public LSurface {
      public:
	JSurface(data::ptr<LSurface::Params> params);
	~JSurface();

	auto roleChanged() -> void override;
	auto orderChanged() -> void override;
	auto minimizedChanged() -> void override;

	/* return the output where the surface is currently most visible */
	auto primaryOutput() -> data::ptr<JOutput>;

	/* take a snapshot of the surface, including its subsurfaces */
	auto capture(data::ptr<LSceneView> sceneView) -> void;

	LSurfaceView view;

	/* a single thumbnail texture shared by all joatbar items */
	data::ptr<LTexture> thumbnail = nullptr;

	/* list of thumbnail views, shared by each output's joatbar */
	std::list<data::ptr<JoatbarItem>> topbarItems;

	/* the output where the surface was most visible before being minimized
	*/
	data::ptr<JOutput> minimizedOutput = nullptr;

	/* the percentage of the surface's position relative to the
	* minimizedOutput before it was minimized
	*/
	LPointF prevMinimizedPos;
};
