#include "jorge.hpp"
#include "joatbar_item.hpp"
#include "joutput.hpp"
#include "data.hpp"
#include "global.hpp"
#include "joatbar.hpp"

Joatbar::Joatbar(data::ptr<JOutput> output)
	: output(output)
	, view(0.0, 0.0, 0.0, 0.8, &J::compositor()->overlayLayer)
{
	/* copy thumbnails from an already initialized output */
	for (auto o : J::outputs()) {
		if (o == output)
			continue;

		for (auto item : (data::ref<std::list<data::ptr<JoatbarItem>>>)o->topbar->view.children())
			new JoatbarItem(this, item->surface);

		break;
	}
	update();
}

Joatbar::~Joatbar()
{
	while (!view.children().empty())
		delete view.children().back();
}

auto Joatbar::update() -> void
{
	view.setSize(output->size().w(), TOPBAR_HEIGHT);
	view.setPos(output->pos());

	/* update thumbnails */
	auto x = TOPBAR_PADDING;
	for (auto item : (data::ref<std::list<data::ptr<JoatbarItem>>>)view.children()) {
		item->setPos(x, TOPBAR_PADDING);
		x += item->size().w() + THUMBNAIL_MARGIN;
	}

	output->repaint();
}
