#pragma once


namespace kl {
	class console {
	public:
		// Clears the console screen
		static void clear() {
			system("cls");
		}

		// Sets the console cursor position
		static void setCursor(kl::point position) {
			SetConsoleCursorPosition(stdConsoleHandle, { short(position.x), short(position.y) });
		}

		// Hides the console cursor
		static void hideCursor() {
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
			cursorInfo.bVisible = FALSE;
			SetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
		}

		// Shows the console cursor
		static void showCursor() {
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
			cursorInfo.bVisible = TRUE;
			SetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
		}

		// Sets the console title
		static void setTitle(std::string text) {
			SetConsoleTitleA(text.c_str());
		}

		// Returns the current console size
		static kl::size getSize() {
			CONSOLE_SCREEN_BUFFER_INFO csbi = {};
			GetConsoleScreenBufferInfo(stdConsoleHandle, &csbi);
			return kl::size(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
		}

		// Returns screen buffer size
		static kl::size getBufferSize() {
			CONSOLE_SCREEN_BUFFER_INFO csbi = {};
			GetConsoleScreenBufferInfo(stdConsoleHandle, &csbi);
			return kl::size(csbi.dwSize.X, csbi.dwSize.Y);
		}

		// Changes the console size
		static void setSize(kl::size size) {
			setBufferSize(size);
			SMALL_RECT consoleRect = { 0, 0, SHORT(size.width - 1), SHORT(size.height - 1) };
			SetConsoleWindowInfo(stdConsoleHandle, TRUE, &consoleRect);
		}

		// Changes the console buffer size
		static void setBufferSize(kl::size size) {
			SetConsoleScreenBufferSize(stdConsoleHandle, { (short)size.width, (short)size.height });
		}

		// Changes the console font size
		static void setFont(kl::size size, std::string fontName = "Consolas") {
			CONSOLE_FONT_INFOEX cfi = {};
			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = SHORT(size.width);
			cfi.dwFontSize.Y = SHORT(size.height);
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;
			wcscpy(cfi.FaceName, kl::convert::toWString(fontName).c_str());
			SetCurrentConsoleFontEx(stdConsoleHandle, FALSE, &cfi);
		}

		// Returns a pressed key
		static char getInput() {
			char input = 0;
			while (_kbhit()) {
				input = _getch();
			}
			return input;
		}

		// Waits until the wanted key is pressed
		static void waitFor(char toWaitFor, bool echo = false) {
			if (echo) {
				if (toWaitFor > 31 && toWaitFor < 127) {
					printf("Press '%c' to continue\n", toWaitFor);
				}
				else {
					printf("Press %d to continue\n", toWaitFor);
				}
			}
			while (_getch() != toWaitFor);
		}

		// Waits for any key to be pressed
		static void waitForAny(bool echo = false) {
			if (echo) {
				printf("Press any key to continue\n");
			}
			char iHateWarnings = _getch();
		}

		// Outputs a progress bar on the console
		static void progressBar(std::string message, kl::uint32 outputY, float percentage) {
			// Prep
			percentage = std::max(std::min(percentage, 1.0f), 0.0f);
			int barLen = console::getSize().width - int(message.length() - 11);
			int doneLen = int(barLen * percentage);
			int emptyLen = barLen - doneLen;

			// Printing
			std::stringstream ss;
			ss << "  " << message << "[";
			for (int i = 0; i < doneLen; i++) {
				ss << '#';
			}
			for (int i = 0; i < emptyLen; i++) {
				ss << ' ';
			}
			console::setCursor(kl::point(0, outputY));
			printf("%s] %3d%% \n", ss.str().c_str(), int(percentage * 100));
		}

		// Fast console writing
		static void fastOut(std::string& data, kl::point location = { 0, 0 }) {
			static DWORD ignore = 0;
			WriteConsoleOutputCharacterA(stdConsoleHandle, data.c_str(), (DWORD)data.length(), { short(location.x), short(location.y) }, &ignore);
		}
		static void fastOut(std::string&& data, kl::point location = { 0, 0 }) {
			fastOut(data, location);
		}

		// Prints RGB data
		static void print(char data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%c\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void print(int data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%d\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void print(kl::int64 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lld\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void print(float data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%f\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void print(double data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lf\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void print(kl::byte data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm0x%02X\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void print(kl::size data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmw: %u h: %u\033[0m", textColor.r, textColor.g, textColor.b, data.width, data.height);
		}
		static void print(kl::point data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %d y: %d\033[0m", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void print(kl::vec2 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf\033[0m", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void print(kl::vec3 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf z: %lf\033[0m", textColor.r, textColor.g, textColor.b, data.x, data.y, data.z);
		}
		static void print(kl::color data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmr: %d g: %d b: %d\033[0m", textColor.r, textColor.g, textColor.b, data.r, data.g, data.b);
		}
		static void print(std::string& data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%s\033[0m", textColor.r, textColor.g, textColor.b, data.c_str());
		}
		static void print(std::string&& data, kl::color textColor = constant::colors::white) {
			print(data, textColor);
		}
		static void print(kl::bytes& data, kl::color textColor = constant::colors::white) {
			print(kl::convert::toString(data), textColor);
		}
		static void print(kl::bytes&& data, kl::color textColor = constant::colors::white) {
			print(kl::convert::toString(data), textColor);
		}

		// Prints RGB data with new line at the end
		static void println(char data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%c\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void println(int data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%d\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void println(kl::int64 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lld\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void println(float data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%f\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void println(double data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lf\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void println(kl::byte data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm0x%02X\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void println(kl::size data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmw: %u h: %u\033[0m\n", textColor.r, textColor.g, textColor.b, data.width, data.height);
		}
		static void println(kl::point data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %d y: %d\033[0m\n", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void println(kl::vec2 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf\033[0m\n", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void println(kl::vec3 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf z: %lf\033[0m\n", textColor.r, textColor.g, textColor.b, data.x, data.y, data.z);
		}
		static void println(kl::color data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmr: %d g: %d b: %d\033[0m\n", textColor.r, textColor.g, textColor.b, data.r, data.g, data.b);
		}
		static void println(std::string& data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%s\033[0m\n", textColor.r, textColor.g, textColor.b, data.c_str());
		}
		static void println(std::string&& data, kl::color textColor = constant::colors::white) {
			println(data, textColor);
		}
		static void println(kl::bytes& data, kl::color textColor = constant::colors::white) {
			println(kl::convert::toString(data), textColor);
		}
		static void println(kl::bytes&& data, kl::color textColor = constant::colors::white) {
			println(kl::convert::toString(data), textColor);
		}

	protected:
		// Enables RGB support for the console
		static void enableRGB() {
			DWORD consoleMode;
			GetConsoleMode(stdConsoleHandle, &consoleMode);
			SetConsoleMode(stdConsoleHandle, consoleMode | 0x04 /*ENABLE_VIRTUAL_TERMINAL_PROCESSING*/);
		}

	private:
		static HANDLE stdConsoleHandle;
	};
	HANDLE console::stdConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}