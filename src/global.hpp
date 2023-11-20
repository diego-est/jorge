#pragma once
#include <LCompositor.h>
#include "data.hpp"

#define TOPBAR_HEIGHT 42
#define TOPBAR_PADDING 4
#define THUMBNAIL_MARGIN 4
#define THUMBNAIL_HEIGHT (TOPBAR_HEIGHT - 2 * TOPBAR_PADDING)

using namespace Louvre;

class JCompositor;
class JOutput;
class JSurface;

class J {
      public:
	/* cast LCompositor to JCompositor */
	static inline auto compositor() -> data::ptr<JCompositor>
	{
		return (data::ptr<JCompositor>)LCompositor::compositor();
	}

	/* cast the LCompositor outputs list from LOutput to JOutput */
	static inline auto outputs() -> data::ref<std::list<data::ptr<JOutput>>>
	{
		return (data::ref<std::list<data::ptr<JOutput>>>)LCompositor::compositor()->outputs();
	}

	/* cast the LCompositor::surfaces() list from LSurface to JSurface */
	static inline auto surfaces() -> data::ref<std::list<data::ptr<JSurface>>>
	{
		return (data::ref<std::list<data::ptr<JSurface>>>)LCompositor::compositor()->surfaces();
	}

	/* move surface views with children */
	static auto moveSurfaceWithChildren(data::ptr<JSurface> surface, data::ptr<LView> parent, bool subSurfacesOnly = false) -> void;

	static auto scene() -> data::ptr<LScene>;
};
