#pragma once

#include "Window/Window.h"
#include "Utility/ASync.h"


namespace kl {
	class DoubleBuffer {
		Image m_Buffer1;
		Image m_Buffer2;
		Image* m_FrontBuffer;
		Mutex m_Lock;

	public:
		DoubleBuffer(const UInt2& frameSize);
		DoubleBuffer(const DoubleBuffer&) = delete;
		void operator=(const DoubleBuffer&) = delete;

		UInt2 getSize() const;

		Image* getBackBuffer();

		void drawToWindow(Window& window);

		void swapBuffers();
	};
}
