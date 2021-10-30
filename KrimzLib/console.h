#pragma once


namespace kl {
	class console {
	public:
		#ifdef _WIN32
		// Deletes the console
		static void Delete() {
			FreeConsole();
		}

		// Clears the console screen
		static void Clear() {
			system("cls");
		}

		// Sets the console cursor position
		static void MoveCursor(point position) {
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (short)position.x, (short)position.y });
		}

		// Hides the console cursor
		static void HideCursor() {
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
			cursorInfo.bVisible = FALSE;
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
		}

		// Shows the console cursor
		static void ShowCursor() {
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
			cursorInfo.bVisible = TRUE;
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
		}

		// Sets the console title
		static void SetTitle(std::string text) {
			SetConsoleTitleA(text.c_str());
		}

		// Returns the current console size
		static size GetSize() {
			CONSOLE_SCREEN_BUFFER_INFO csbi = {};
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			return {
				int(csbi.srWindow.Right - csbi.srWindow.Left + 1),
				int(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
			};
		}

		// Returns screen buffer size
		static size GetBufferSize() {
			CONSOLE_SCREEN_BUFFER_INFO csbi = {};
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			return { csbi.dwSize.X, csbi.dwSize.Y };
		}

		// Changes the console size
		static void SetSize(size size) {
			SetBufferSize(size);
			SMALL_RECT consoleRect = { 0, 0, (short)size.width - 1, (short)size.height - 1 };
			SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &consoleRect);
		}

		// Changes the console buffer size
		static void SetBufferSize(size size) {
			SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), { (short)size.width, (short)size.height });
		}

		// Changes the console font size
		static void SetFont(size size, std::string fontName = "Consolas") {
			CONSOLE_FONT_INFOEX cfi = {};
			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = size.width;
			cfi.dwFontSize.Y = size.height;
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;
			wcscpy(cfi.FaceName, convert::ToWString(fontName).c_str());
			SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
		}

		// Enables RGB support for the console
		static void EnableRGB() {
			static bool rgbEnabled = false;
			if (!rgbEnabled) {
				DWORD consoleMode;
				GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &consoleMode);
				SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
				rgbEnabled = true;
			}
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
		#endif

		// Prints RGB data
		static void Print(char data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%c\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(int data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%d\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(int64 data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lld\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(float data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%f\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(double data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lf\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(byte data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm0x%02X\033[0m", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(size data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmw: %d h: %d\033[0m", textColor.r, textColor.g, textColor.b, data.width, data.height);
		}
		static void Print(point data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %d y: %d\033[0m", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void Print(vec2 data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf\033[0m", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void Print(vec3 data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf z: %lf\033[0m", textColor.r, textColor.g, textColor.b, data.x, data.y, data.z);
		}
		static void Print(color data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmr: %d g: %d b: %d\033[0m", textColor.r, textColor.g, textColor.b, data.r, data.g, data.b);
		}
		static void Print(std::string& data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%s\033[0m", textColor.r, textColor.g, textColor.b, data.c_str());
		}
		static void Print(std::string&& data, color textColor = constant::colors::white) {
			Print(data, textColor);
		}

		// Prints RGB data with new line at the end
		static void Println(char data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%c\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(int data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%d\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(int64 data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lld\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(float data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%f\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(double data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%lf\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(byte data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm0x%02X\033[0m\n", textColor.r, textColor.g, textColor.b, data);
		}
		static void Println(size data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmw: %d h: %d\033[0m\n", textColor.r, textColor.g, textColor.b, data.width, data.height);
		}
		static void Println(point data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %d y: %d\033[0m\n", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void Println(vec2 data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf\033[0m\n", textColor.r, textColor.g, textColor.b, data.x, data.y);
		}
		static void Println(vec3 data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmx: %lf y: %lf z: %lf\033[0m\n", textColor.r, textColor.g, textColor.b, data.x, data.y, data.z);
		}
		static void Println(color data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dmr: %d g: %d b: %d\033[0m\n", textColor.r, textColor.g, textColor.b, data.r, data.g, data.b);
		}
		static void Println(std::string& data, color textColor = constant::colors::white) {
			printf("\033[38;2;%d;%d;%dm%s\033[0m\n", textColor.r, textColor.g, textColor.b, data.c_str());
		}
		static void Println(std::string&& data, color textColor = constant::colors::white) {
			Println(data, textColor);
		}

		// Prints RGB block
		static void PrintCell(color blockColor) {
			printf("\033[48;2;%d;%d;%dm \033[0m", blockColor.r, blockColor.g, blockColor.b);
		}
	};
}
