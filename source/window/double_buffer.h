#pragma once

#include <mutex>

#include "window/window.h"


namespace kl {
	class double_buffer {
	private:
		kl::image m_Buffer1;
		kl::image m_Buffer2;
		kl::image* m_FrontBuffer;
		std::mutex m_Lock;

	public:
		double_buffer(const kl::uint2& frameSize);
		double_buffer(const double_buffer&) = delete;
		void operator=(const double_buffer&) = delete;

		kl::uint2 size();

		kl::image* getBB();

		void draw(kl::window& win);
		void operator>>(kl::window& win);

		void swap();
	};
}
