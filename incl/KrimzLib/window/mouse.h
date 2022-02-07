#pragma once

#include <windows.h>

#include "KrimzLib/vecmath/ivec2.h"
#include "KrimzLib/vecmath/vec2.h"


namespace kl {
    class mouse {
	private:
		HWND hwnd = nullptr;
		int showCounter = 0;

	public:
		bool lmb = false;
		bool mmb = false;
		bool rmb = false;
		kl::ivec2 position;

		// Constructor
		mouse();

		// Binds the mouse to the window
		void bind(HWND hwnd);

		// Hides the mouse cursor
		void hide();

		// Shows the mouse cursor
		void show();

		// Moves the mouse pointer
		void move(const kl::ivec2& pos);

		// Return a normalized screen position
		kl::vec2 normPos(const kl::ivec2& frameSize) const;
	};
}
