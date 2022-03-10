#pragma once

#include <windows.h>

#include "KrimzLib/math/int2.h"
#include "KrimzLib/math/float2.h"


namespace kl {
    class mouse {
	private:
		HWND hwnd = nullptr;
		int showCounter = 0;

	public:
		bool lmb = false;
		bool mmb = false;
		bool rmb = false;
		kl::int2 position;
		int scroll = 0;

		// Constructor
		mouse();

		// Binds the mouse to the window
		void bind(HWND hwnd);

		// Hides the mouse cursor
		void hide();

		// Shows the mouse cursor
		void show();

		// Moves the mouse pointer
		void move(const kl::int2& pos);

		// Return a normalized screen position
		kl::float2 normPos(const kl::int2& frameSize) const;
	};
}
