#include "utility/console.h"
#include "utility/strings.h"
#include "math/math.h"

#include <iostream>
#include <sstream>
#include <conio.h>
#include <windows.h>

#undef min
#undef max


static const HANDLE consoleHandle = []()
{
	HANDLE tempHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode = {};
	GetConsoleMode(tempHandle, &consoleMode);
	SetConsoleMode(tempHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	return tempHandle;
}();

void kl::console::clear()
{
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenInfo = {};
	GetConsoleScreenBufferInfo(consoleHandle, &consoleScreenInfo);

	DWORD charsWritten = {};
	FillConsoleOutputCharacterA(consoleHandle, ' ', consoleScreenInfo.dwSize.X * consoleScreenInfo.dwSize.Y, {}, &charsWritten);

	FillConsoleOutputAttribute(
		consoleHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		consoleScreenInfo.dwSize.X * consoleScreenInfo.dwSize.Y, {}, &charsWritten
	);
	kl::console::moveCursor({});
}

void kl::console::hide()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void kl::console::show()
{
	ShowWindow(GetConsoleWindow(), SW_SHOW);
}

void kl::console::moveCursor(const kl::uint2& position)
{
	SetConsoleCursorPosition(consoleHandle, { short(position.x), short(position.y) });
}

void kl::console::hideCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo = {};
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void kl::console::showCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo = {};
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void kl::console::title(const std::string& text)
{
	SetConsoleTitleA(text.c_str());
}

kl::uint2 kl::console::size()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi = {};
	GetConsoleScreenBufferInfo(consoleHandle, &csbi);
	return kl::uint2(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

void kl::console::resize(const kl::uint2& size)
{
	SMALL_RECT consoleRect = { 0, 0, SHORT(size.x - 1), SHORT(size.y - 1) };
	SetConsoleWindowInfo(consoleHandle, true, &consoleRect);
}

void kl::console::font(const kl::uint2& size, const std::string& fontName)
{
	CONSOLE_FONT_INFOEX cfi = {};
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = SHORT(size.x);
	cfi.dwFontSize.Y = SHORT(size.y);
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, kl::to::wstring(fontName).c_str());
	SetCurrentConsoleFontEx(consoleHandle, false, &cfi);
}

char kl::console::input()
{
	char input = 0;
	while (_kbhit())
	{
		input = _getch();
	}
	return input;
}

void kl::console::waitFor(char toWaitFor, bool echo)
{
	if (echo)
	{
		if (toWaitFor > 31 && toWaitFor < 127)
		{
			kl::print("Press '", toWaitFor, "' to continue...");
		}
		else
		{
			kl::print("Press '", int(toWaitFor), "' to continue...");
		}
	}

	char c = 0;
	while ((c = _getch()) != toWaitFor) kl::print(int(c));
}

char kl::console::waitForAny(bool echo)
{
	if (echo)
	{
		kl::print("Press any key to continue...");
	}
	return _getch();
}

bool kl::console::warning(bool occured, const std::string& message, bool wait)
{
	if (occured)
	{
		kl::console::show();
		kl::print(kl::colors::orange, "Warning: ", message);
		if (wait)
		{
			kl::console::waitForAny(false);
		}
		kl::print<kl::none>(kl::colors::defaul);
	}
	return occured;
}

void kl::console::error(bool occured, const std::string& message, bool wait)
{
	if (occured)
	{
		kl::console::show();
		kl::print(kl::colors::red, "Error: ", message);
		if (wait)
		{
			kl::console::waitForAny(false);
		}
		kl::print<kl::none>(kl::colors::defaul);
		exit(1);
	}
}

static DWORD ignore = 0;
void kl::console::dump(const std::string& data, const kl::uint2& location)
{
	WriteConsoleOutputCharacterA(consoleHandle, data.c_str(), DWORD(data.length()), { short(location.x), short(location.y) }, &ignore);
}

void kl::console::bar(const std::string& message, uint outputY, float percentage)
{
	percentage = kl::math::minmax(percentage, 0.0f, 1.0f);
	const int barLen = kl::console::size().x - int(message.length()) - 12;
	const int finishLen = int(barLen * percentage);
	const int emptyLen = barLen - finishLen;

	std::stringstream ss;
	ss << "  " << message << " [";
	for (int i = 0; i < finishLen; i++)
	{
		ss << '#';
	}
	for (int i = 0; i < emptyLen; i++)
	{
		ss << ' ';
	}
	kl::console::moveCursor(kl::uint2(0, outputY));
	printf("%s] %3d%% ", ss.str().c_str(), int(percentage * 100.0f));
}
