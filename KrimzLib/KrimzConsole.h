#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "KrimzLib/KrimzTypes.h"
#include "KrimzLib/KrimzConstants.h"


namespace kl {
	class console {
	public:
		// Sets the console title
		static void SetTitle(std::string text) {
			SetConsoleTitleA(text.c_str());
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

		// Sets the console cursor position
		static void SetCursorPos(short x, short y) {
			SetConsoleCursorPosition(stdConsoleHandle, { x, y });
		}
		static void SetCursorPos(COORD position) {
			SetConsoleCursorPosition(stdConsoleHandle, position);
		}

		// Returns the current console size
		static size GetSize() {
			CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
			GetConsoleScreenBufferInfo(stdConsoleHandle, &consoleScreenBufferInfo);
			return {
				int(consoleScreenBufferInfo.srWindow.Right) - int(consoleScreenBufferInfo.srWindow.Left) + 1,
				int(consoleScreenBufferInfo.srWindow.Bottom) - int(consoleScreenBufferInfo.srWindow.Top) + 1
			};
		}

		// Changes the console buffer size
		static void SetBufferSize(int width, int height) {
			SetConsoleScreenBufferSize(stdConsoleHandle, { (short)width, (short)height });
		}

		// Changes the console size
		static void SetSize(int width, int height) {
			SetBufferSize(width, height);
			SMALL_RECT consoleRect = { 0, 0, (short)width - 1, (short)height - 1 };
			SetConsoleWindowInfo(stdConsoleHandle, TRUE, &consoleRect);
		}

		// Changes the console font size
		static void SetFont(int width, int height, std::wstring fontName) {
			CONSOLE_FONT_INFOEX cfi;
			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = width;
			cfi.dwFontSize.Y = height;
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;
			wcscpy_s(cfi.FaceName, fontName.c_str());
			SetCurrentConsoleFontEx(stdConsoleHandle, FALSE, &cfi);
		}

		// Enables RGB support for the console
		static void EnableRGB() {
			if (!rgbEnabled) {
				// Get current console mode
				DWORD consoleMode;
				GetConsoleMode(stdConsoleHandle, &consoleMode);
				lastConsoleMode = consoleMode;

				// Enable the RGB
				consoleMode = consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				SetConsoleMode(stdConsoleHandle, consoleMode);
				rgbEnabled = true;
			}
		}

		// Disables RGB support for the console
		static void DisableRGB() {
			if (rgbEnabled) {
				SetConsoleMode(stdConsoleHandle, lastConsoleMode);
				rgbEnabled = false;
			}
		}

		// Prints RGB text
		static void Print(std::string data, color textColor = constant::colorWhite) {
			printf("\x1b[38;2;%d;%d;%dm%s", textColor.r, textColor.g, textColor.b, data.c_str());
		}
		static void Print(int data, color textColor = constant::colorWhite) {
			printf("\x1b[38;2;%d;%d;%dm%d", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(double data, color textColor = constant::colorWhite) {
			printf("\x1b[38;2;%d;%d;%dm%lf", textColor.r, textColor.g, textColor.b, data);
		}

		// Prints RGB block
		static void PrintBlock(byte r, byte g, byte b) {
			printf("\x1b[48;2;%d;%d;%dm ", r, g, b);
		}
		static void PrintBlock(color blockColor) {
			printf("\x1b[48;2;%d;%d;%dm ", blockColor.r, blockColor.g, blockColor.b);
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
		static void WaitFor(char toWaitFor, bool printMessage = false) {
			if (printMessage) {
				if (toWaitFor > 31 && toWaitFor < 127) {
					printf("Waiting for '%c'\n", toWaitFor);
				}
				else {
					printf("Waiting for '%d'\n", toWaitFor);
				}
			}
			while (_getch() != toWaitFor);
		}

	private:
		static HANDLE stdConsoleHandle;
		static bool rgbEnabled;
		static DWORD lastConsoleMode;
	};
	HANDLE console::stdConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	bool console::rgbEnabled = false;
	DWORD console::lastConsoleMode = 0;
}
