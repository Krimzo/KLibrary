#pragma once

#include <mutex>

#include "graphics/image.h"
#include "window/window.h"


namespace kl {
	class double_buffer {
	private:
		kl::image buff1;
		kl::image buff2;
		kl::image* fb;
		std::mutex lock;

	public:
		double_buffer(const kl::int2& frameSize);
		double_buffer(const double_buffer&) = delete;

		kl::int2 size();

		kl::image* getBB();

		void draw(kl::window& win);
		void operator>>(kl::window& win);

		void swap();
	};
}
