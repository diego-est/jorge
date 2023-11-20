#include "global.hpp"
#include "jorge.hpp"
#include "jsurface.hpp"

#include <LCompositor.h>

auto J::scene() -> data::ptr<LScene>
{
	return &compositor()->scene;
}

auto J::moveSurfaceWithChildren(data::ptr<JSurface> surface, data::ptr<LView> parent, bool subSurfacesOnly) -> void
{
	surface->view.setParent(parent);
	auto next = surface;

	if(subSurfacesOnly) {
		while ((next = (data::ptr<JSurface>)next->nextSurface()))
			if (next->isSubchildOf(surface) && next->subsurface())
				next->view.setParent(parent);
	} else {
		while ((next = (data::ptr<JSurface>)next->nextSurface()))
			if (next->isSubchildOf(surface))
				next->view.setParent(parent);
	}

}
