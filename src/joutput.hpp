#pragma once
#include <memory>
#include <LOutput.h>
#include "data.hpp"

using namespace Louvre;

class Joatbar;

class JOutput : public LOutput {
      public:
	JOutput();

	auto initializeGL() -> void override;
	auto moveGL() -> void override;
	auto resizeGL() -> void override;
	auto paintGL() -> void override;
	auto uninitializeGL() -> void override;

	data::ptr<LTextureView> wallpaperView = nullptr;
	data::ptr<Joatbar> topbar = nullptr;
	auto updateWallpaper() -> void;
};
