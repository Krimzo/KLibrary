#pragma once

#include <windows.h>

#include "math/int2.h"
#include "math/float2.h"
#include "window/keys.h"


namespace kl
{
	class mouse
	{
	private:
		HWND hwnd = nullptr;
		int showCounter = 0;

	public:
		kl::key lmb;
		kl::key mmb;
		kl::key rmb;
		kl::int2 position;
		int scroll = 0;

		// Binds the mouse to the window
		void bind(HWND hwnd);

		// Calls all key down functions
		void callAllDowns();

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
