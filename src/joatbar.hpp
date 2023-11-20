#pragma once
#include <LSolidColorView.h>
#include <memory>
#include "data.hpp"

using namespace Louvre;

class JOutput;

class Joatbar : public LObject {
      public:
	Joatbar(data::ptr<JOutput> output);
	~Joatbar();

	data::ptr<JOutput> output = nullptr;
	LSolidColorView view;

	auto update() -> void;
};

