#include "joatbar_item.hpp"
#include "jsurface.hpp"
#include "joatbar.hpp"

JoatbarItem::JoatbarItem(data::ptr<Joatbar> topbar, data::ptr<JSurface> surface)
	: LTextureView(surface->thumbnail, &topbar->view)
	, topbar(topbar)
	, surface(surface)
{
	surface->topbarItems.push_back(this);
	surfaceLink = std::prev(surface->topbarItems.end());

	setBufferScale(2);
	enableParentOpacity(false);
	topbar->update();
}

JoatbarItem::~JoatbarItem()
{
	surface->topbarItems.erase(surfaceLink);
	setParent(nullptr);
	topbar->update();
}
