#include "utility/console.h"
#include "utility/encrypter.h"
#include "math/math.h"

#include <iostream>
#include <sstream>
#include <conio.h>
#include <windows.h>


static const HANDLE consoleHandle = []() {
	HANDLE tempHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode = {};
	GetConsoleMode(tempHandle, &consoleMode);
	SetConsoleMode(tempHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	return tempHandle;
}();

// Clears the console screen
void kl::console::clear() {
	system("cls");
}

// Hides the console
void kl::console::hide() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

// Shows the console
void kl::console::show() {
	ShowWindow(GetConsoleWindow(), SW_SHOW);
}

// Sets the console cursor position
void kl::console::moveCursor(const kl::int2& position) {
	SetConsoleCursorPosition(consoleHandle, { short(position.x), short(position.y) });
}

// Hides the console cursor
void kl::console::hideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo = {};
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

// Shows the console cursor
void kl::console::showCursor() {
	CONSOLE_CURSOR_INFO cursorInfo = {};
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

// Sets the console title
void kl::console::setTitle(const std::string& text) {
	SetConsoleTitleA(text.c_str());
}

// Returns the current console size
kl::int2 kl::console::size() {
	CONSOLE_SCREEN_BUFFER_INFO csbi = {};
	GetConsoleScreenBufferInfo(consoleHandle, &csbi);
	return kl::int2(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

// Changes the console size
void kl::console::resize(const kl::int2& size) {
	SMALL_RECT consoleRect = { 0, 0, SHORT(size.x - 1), SHORT(size.y - 1) };
	SetConsoleWindowInfo(consoleHandle, true, &consoleRect);
}

// Changes the console font size
void kl::console::setFont(const kl::int2& size, const std::string& fontName) {
	CONSOLE_FONT_INFOEX cfi = {};
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = SHORT(size.x);
	cfi.dwFontSize.Y = SHORT(size.y);
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, kl::toWString(fontName).c_str());
	SetCurrentConsoleFontEx(consoleHandle, false, &cfi);
}

// Returns a pressed key
char kl::console::input() {
	char input = 0;
	while (_kbhit()) {
		input = _getch();
	}
	return input;
}

// Waits until the wanted key is pressed
void kl::console::waitFor(char toWaitFor, bool echo) {
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
char kl::console::waitForAny(bool echo) {
	if (echo) {
		printf("Press any key to continue\n");
	}
	return _getch();
}

// Outputs a progress bar on the console
void kl::console::progressBar(const std::string& message, int outputY, float percentage) {
	// Prep
	percentage = max(min(percentage, 1.0f), 0.0f);
	const int barLen = console::size().x - int(message.length()) - 12;
	const int finishLen = int(barLen * percentage);
	const int emptyLen = barLen - finishLen;

	// Printing
	std::stringstream ss;
	ss << "  " << message << " [";
	for (int i = 0; i < finishLen; i++) {
		ss << '#';
	}
	for (int i = 0; i < emptyLen; i++) {
		ss << ' ';
	}
	console::moveCursor(kl::int2(0, outputY));
	printf("%s] %3d%% ", ss.str().c_str(), int(percentage * 100.0f));
}

// Fast console writing
static DWORD ignore = 0;
void kl::console::fastOut(const std::string& data, const kl::int2& location) {
	WriteConsoleOutputCharacterA(consoleHandle, data.c_str(), DWORD(data.length()), { short(location.x), short(location.y) }, &ignore);
}
