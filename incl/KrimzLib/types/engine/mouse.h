#pragma once


namespace kl {
    struct mouse {
		bool lmb;
		bool mmb;
		bool rmb;
		kl::point position;

		// Constructor
		mouse() {
			lmb = false;
			mmb = false;
			rmb = false;
			position = kl::point();
			hwnd = nullptr;
			showCounter = 0;
		}

		// Binds the mouse to the window
		void bind(HWND hwnd) {
			this->hwnd = hwnd;
		}

		// Hides the mouse cursor
		void hide() {
			while (showCounter > -1) {
				showCounter = ShowCursor(FALSE);
			}
		}

		// Shows the mouse cursor
		void show() {
			while (showCounter < 0) {
				showCounter = ShowCursor(TRUE);
			}
		}

		// Moves the mouse pointer
		void move(kl::point pos) {
			POINT clientPos = { pos.x, pos.y };
			ClientToScreen(hwnd, &clientPos);
			SetCursorPos(clientPos.x, clientPos.y);
		}
		
	private:
		HWND hwnd;
		int showCounter;
	};
}