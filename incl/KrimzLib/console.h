#pragma once


namespace kl {
	class console {
	public:
		// Deletes the console
		static void Delete() {
			FreeConsole();
		}

		// Clears the console screen
		static void Clear() {
			system("cls");
		}

		// Sets the console cursor position
		static void MoveCursor(kl::point position) {
			SetConsoleCursorPosition(stdConsoleHandle, { (short)position.x, (short)position.y });
		}

		// Hides the console cursor
		static void HideCursor() {
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
			cursorInfo.bVisible = FALSE;
			SetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
		}

		// Shows the console cursor
		static void ShowCursor() {
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
			cursorInfo.bVisible = TRUE;
			SetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
		}

		// Sets the console title
		static void SetTitle(std::string text) {
			SetConsoleTitleA(text.c_str());
		}

		// Returns the current console size
		static kl::size GetSize() {
			CONSOLE_SCREEN_BUFFER_INFO csbi = {};
			GetConsoleScreenBufferInfo(stdConsoleHandle, &csbi);
			return {
				kl::uint32(csbi.srWindow.Right - csbi.srWindow.Left + 1),
				kl::uint32(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
			};
		}

		// Returns screen buffer size
		static kl::size GetBufferSize() {
			CONSOLE_SCREEN_BUFFER_INFO csbi = {};
			GetConsoleScreenBufferInfo(stdConsoleHandle, &csbi);
			return {
				kl::uint32(csbi.dwSize.X),
				kl::uint32(csbi.dwSize.Y)
			};
		}

		// Changes the console size
		static void SetSize(kl::size size) {
			SetBufferSize(size);
			SMALL_RECT consoleRect = { 0, 0, SHORT(size.width - 1), SHORT(size.height - 1) };
			SetConsoleWindowInfo(stdConsoleHandle, TRUE, &consoleRect);
		}

		// Changes the console buffer size
		static void SetBufferSize(kl::size size) {
			SetConsoleScreenBufferSize(stdConsoleHandle, { (short)size.width, (short)size.height });
		}

		// Changes the console font size
		static void SetFont(kl::size size, std::string fontName = "Consolas") {
			CONSOLE_FONT_INFOEX cfi = {};
			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = (SHORT)size.width;
			cfi.dwFontSize.Y = (SHORT)size.height;
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;
			wcscpy(cfi.FaceName, kl::convert::ToWString(fontName).c_str());
			SetCurrentConsoleFontEx(stdConsoleHandle, FALSE, &cfi);
		}

		// Returns a pressed key
		static char GetInput() {
			char input = 0;
			while (_kbhit()) {
				input = _getch();
			}
			return input;
		}

		// Waits until the wanted key is pressed
		static void WaitFor(char toWaitFor, bool echo = false) {
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
		static void WaitForAny(bool echo = false) {
			if (echo) {
				printf("Press any key to continue\n");
			}
			char iHateWarnings = _getch();
		}

		// Outputs a progress bar on the console
		static void ProgressBar(std::string message, kl::uint32 outputY, double percentage) {
			// Prep
			percentage = std::max(std::min(percentage, 1.0), 0.0);
			int barLen = console::GetSize().width - (int)message.length() - 11;
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
			console::MoveCursor(kl::point(0, outputY));
			printf("%s] %3d%% \n", ss.str().c_str(), int(percentage * 100));
		}

		// Fast console writing
		static void FastOut(std::string& data, kl::point location = { 0, 0 }) {
			static DWORD ignore = 0;
			WriteConsoleOutputCharacterA(stdConsoleHandle, data.c_str(), (DWORD)data.length(), { (short)location.x, (short)location.y }, &ignore);
		}
		static void FastOut(std::string&& data, kl::point location = { 0, 0 }) {
			FastOut(data, location);
		}

		// Prints RGB data
		static void Print(char data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%c\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(int data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%d\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(kl::int64 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lld\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(float data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%f\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(double data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lf\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(kl::byte data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm0x%02X\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(kl::size data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmw: %u h: %u\033[0m", textColor.r, textColor.g, textColor.b, data.width, data.height);
		}
		static void Print(kl::point data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %d y: %d\033[0m", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void Print(kl::vec2 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf\033[0m", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void Print(kl::vec3 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf z: %lf\033[0m", textColor.r, textColor.g, textColor.b, data.x, data.y, data.z);
		}
		static void Print(kl::color data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmr: %d g: %d b: %d\033[0m", textColor.r, textColor.g, textColor.b, data.r, data.g, data.b);
		}
		static void Print(std::string& data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%s\033[0m", textColor.r, textColor.g, textColor.b, data.c_str());
		}
		static void Print(std::string&& data, kl::color textColor = constant::colors::white) {
			Print(data, textColor);
		}
		static void Print(kl::bytes& data, kl::color textColor = constant::colors::white) {
			Print(kl::convert::ToString(data), textColor);
		}
		static void Print(kl::bytes&& data, kl::color textColor = constant::colors::white) {
			Print(kl::convert::ToString(data), textColor);
		}

		// Prints RGB data with new line at the end
		static void Println(char data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%c\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(int data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%d\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(kl::int64 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lld\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(float data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%f\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(double data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lf\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(kl::byte data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm0x%02X\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(kl::size data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmw: %u h: %u\033[0m\n", textColor.r, textColor.g, textColor.b, data.width, data.height);
		}
		static void Println(kl::point data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %d y: %d\033[0m\n", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void Println(kl::vec2 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf\033[0m\n", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void Println(kl::vec3 data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf z: %lf\033[0m\n", textColor.r, textColor.g, textColor.b, data.x, data.y, data.z);
		}
		static void Println(kl::color data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmr: %d g: %d b: %d\033[0m\n", textColor.r, textColor.g, textColor.b, data.r, data.g, data.b);
		}
		static void Println(std::string& data, kl::color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%s\033[0m\n", textColor.r, textColor.g, textColor.b, data.c_str());
		}
		static void Println(std::string&& data, kl::color textColor = constant::colors::white) {
			Println(data, textColor);
		}
		static void Println(kl::bytes& data, kl::color textColor = constant::colors::white) {
			Println(kl::convert::ToString(data), textColor);
		}
		static void Println(kl::bytes&& data, kl::color textColor = constant::colors::white) {
			Println(kl::convert::ToString(data), textColor);
		}

	protected:
		// Enables RGB support for the console
		static void EnableRGB() {
			DWORD consoleMode;
			GetConsoleMode(stdConsoleHandle, &consoleMode);
			SetConsoleMode(stdConsoleHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		}

	private:
		static HANDLE stdConsoleHandle;
	};
	HANDLE console::stdConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}