#pragma once


namespace kl {
    struct mouse {
		bool lmb = false;
		bool mmb = false;
		bool rmb = false;
		kl::ivec2 position;

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
		void move(kl::ivec2 pos) {
			POINT clientPos = { pos.x, pos.y };
			ClientToScreen(hwnd, &clientPos);
			SetCursorPos(clientPos.x, clientPos.y);
		}

		// Return a normalized screen position
		kl::vec2 normPos(kl::ivec2 frameSize) {
			kl::vec2 pos(
				float(position.x) / frameSize.x,
				float(frameSize.y - position.y) / frameSize.y
			);
			pos *= 2;
			pos -= 1;
			return pos;
		}
		
	private:
		HWND hwnd = nullptr;
		int showCounter = 0;
	};
}