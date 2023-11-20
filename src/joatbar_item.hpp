#pragma once
#include <LTextureView.h>
#include "data.hpp"

using namespace Louvre;

class Joatbar;
class JSurface;

class JoatbarItem : public LTextureView {
      public:
	JoatbarItem(data::ptr<Joatbar> topbar, data::ptr<JSurface> surface);
	~JoatbarItem();

	std::list<data::ptr<JoatbarItem>>::iterator surfaceLink;

	data::ptr<Joatbar> topbar = nullptr;
	data::ptr<JSurface> surface = nullptr;
};
