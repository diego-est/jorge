#pragma once

namespace data {
	template<typename T>
	using view_ptr = const T*;

	template<typename T>
	using ptr = T*;

	template<typename T>
	using ref = T&;
}
