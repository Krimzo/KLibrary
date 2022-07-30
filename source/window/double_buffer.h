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

		kl::uint2 size() const;

		kl::image* bbuffer();

		void draw(kl::window& window);
		void operator>>(kl::window& window);

		void swap();
	};
}
