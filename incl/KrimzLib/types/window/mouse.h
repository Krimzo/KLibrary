#pragma once


namespace kl {
    struct mouse {
		bool lmb = false;
		bool mmb = false;
		bool rmb = false;
		kl::point position;

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
		HWND hwnd = nullptr;
		int showCounter = 0;
	};
}