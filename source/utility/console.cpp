#define _CRT_SECURE_NO_WARNINGS
#include "utility/console.h"

#include <iostream>
#include <sstream>
#include <windows.h>

#include "math/int2.h"
#include "color/color.h"
#include "utility/convert.h"


// Getting the console handle and rgb init
HANDLE kl::console::handle = []()
{
	// Getting the standard console handle
	HANDLE tempHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Enabling the console RGB
	DWORD consoleMode = {};
	GetConsoleMode(tempHandle, &consoleMode);
	SetConsoleMode(tempHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

	// Returning the handle
	return tempHandle;
}();

// Clears the console screen
void kl::console::clear()
{
	system("cls");
}

// Hides the console
void kl::console::hide()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

// Shows the console
void kl::console::show()
{
	ShowWindow(GetConsoleWindow(), SW_SHOW);
}

// Sets the console cursor position
void kl::console::setCursor(const kl::int2& position)
{
	SetConsoleCursorPosition(kl::console::handle, { short(position.x), short(position.y) });
}

// Hides the console cursor
void kl::console::hideCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo = {};
	GetConsoleCursorInfo(kl::console::handle, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(kl::console::handle, &cursorInfo);
}

// Shows the console cursor
void kl::console::showCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo = {};
	GetConsoleCursorInfo(kl::console::handle, &cursorInfo);
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(kl::console::handle, &cursorInfo);
}

// Sets the console title
void kl::console::setTitle(const std::string& text)
{
	SetConsoleTitleA(text.c_str());
}

// Returns screen buffer size
kl::int2 kl::console::getBufferSize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi = {};
	GetConsoleScreenBufferInfo(kl::console::handle, &csbi);
	return kl::int2(csbi.dwSize.X, csbi.dwSize.Y);
}

// Returns the current console size
kl::int2 kl::console::getSize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi = {};
	GetConsoleScreenBufferInfo(kl::console::handle, &csbi);
	return kl::int2(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

// Changes the console buffer size
void kl::console::setBufferSize(const kl::int2& size)
{
	SetConsoleScreenBufferSize(kl::console::handle, { (short)size.x, (short)size.y });
}

// Changes the console size
void kl::console::setSize(const kl::int2& size)
{
	setBufferSize(size);
	SMALL_RECT consoleRect = { 0, 0, SHORT(size.x - 1), SHORT(size.y - 1) };
	SetConsoleWindowInfo(kl::console::handle, true, &consoleRect);
}

// Changes the console font size
void kl::console::setFont(const kl::int2& size, const std::string& fontName)
{
	CONSOLE_FONT_INFOEX cfi = {};
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = SHORT(size.x);
	cfi.dwFontSize.Y = SHORT(size.y);
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, kl::convert::toWString(fontName).c_str());
	SetCurrentConsoleFontEx(kl::console::handle, false, &cfi);
}

// Returns a pressed key
char kl::console::getInput()
{
	char input = 0;
	while (_kbhit())
		input = _getch();
	return input;
}

// Waits until the wanted key is pressed
void kl::console::waitFor(char toWaitFor, bool echo)
{
	if (echo)
	{
		if (toWaitFor > 31 && toWaitFor < 127)
			printf("Press '%c' to continue\n", toWaitFor);
		else
			printf("Press %d to continue\n", toWaitFor);
	}
	while (_getch() != toWaitFor);
}

// Waits for any key to be pressed
char kl::console::waitForAny(bool echo)
{
	if (echo)
		printf("Press any key to continue\n");
	return _getch();
}

// Outputs a progress bar on the console
void kl::console::progressBar(const std::string& message, int outputY, float percentage)
{
	// Prep
	percentage = max(min(percentage, 1.0f), 0.0f);
	const int barLen = console::getSize().x - int(message.length()) - 12;
	const int finishLen = int(barLen * percentage);
	const int emptyLen = barLen - finishLen;

	// Printing
	std::stringstream ss;
	ss << "  " << message << " [";
	for (int i = 0; i < finishLen; i++)
		ss << '#';
	for (int i = 0; i < emptyLen; i++)
		ss << ' ';
	console::setCursor(kl::int2(0, outputY));
	printf("%s] %3d%% ", ss.str().c_str(), int(percentage * 100.0f));
}

// Fast console writing
DWORD ignore = 0;
void kl::console::fastOut(const std::string& data, const kl::int2& location)
{
	WriteConsoleOutputCharacterA(kl::console::handle, data.c_str(), (DWORD)data.length(), { short(location.x), short(location.y) }, &ignore);
}
