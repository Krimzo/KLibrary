#include "Utility/Console.h"
#include "Utility/Strings.h"
#include "Math/Math.h"

#include <iostream>
#include <sstream>
#include <conio.h>
#include <windows.h>

#undef min
#undef max


static const HANDLE ConsoleHandle = []() {
	HANDLE tempHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode = {};
	GetConsoleMode(tempHandle, &consoleMode);
	SetConsoleMode(tempHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	return tempHandle;
}();

void kl::Get() {
	std::cin.get();
}

bool kl::Warning(bool occured, const String& message, bool wait) {
	if (occured) {
		Console::SetEnabled(true);
		Print(Colors::Orange, "Warning: ", message);
		if (wait) {
			Console::WaitAny();
		}
		Print<false>(Colors::Default);
	}
	return occured;
}

void kl::Assert(bool occured, const String& message, bool wait) {
	if (occured) {
		Console::SetEnabled(true);
		Print(Colors::Red, "Error: ", message);
		if (wait) {
			Console::WaitAny();
		}
		Print<false>(Colors::Default);
		exit(1);
	}
}

void kl::Console::SetEnabled(bool state) {
	static HWND consoleWindow = GetConsoleWindow();
	if (state) {
		ShowWindow(consoleWindow, SW_SHOW);

	}
	else {
		ShowWindow(consoleWindow, SW_HIDE);
	}
}

void kl::Console::Clear() {
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenInfo = {};
	GetConsoleScreenBufferInfo(ConsoleHandle, &consoleScreenInfo);

	DWORD charsWritten = {};
	FillConsoleOutputCharacterA(ConsoleHandle, ' ', consoleScreenInfo.dwSize.X * consoleScreenInfo.dwSize.Y, {}, &charsWritten);

	FillConsoleOutputAttribute(
		ConsoleHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		consoleScreenInfo.dwSize.X * consoleScreenInfo.dwSize.Y, {}, &charsWritten
	);

	MoveCursor({});
}

void kl::Console::SetCursorEnabled(bool state) {
	CONSOLE_CURSOR_INFO cursorInfo = {};
	GetConsoleCursorInfo(ConsoleHandle, &cursorInfo);
	cursorInfo.bVisible = state;
	SetConsoleCursorInfo(ConsoleHandle, &cursorInfo);
}

void kl::Console::MoveCursor(const UInt2& position) {
	SetConsoleCursorPosition(ConsoleHandle, { short(position.x), short(position.y) });
}

kl::uint kl::Console::GetWidth() {
	return GetSize().x;
}

void kl::Console::SetWidth(uint width) {
	SetSize({ width, GetHeight() });
}

kl::uint kl::Console::GetHeight() {
	return GetSize().y;
}

void kl::Console::SetHeight(uint height) {
	SetSize({ GetWidth(), height });
}

void kl::Console::SetTitle(const String& text) {
	SetConsoleTitleA(text.c_str());
}

kl::UInt2 kl::Console::GetSize() {
	CONSOLE_SCREEN_BUFFER_INFO csbi = {};
	GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
	return { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
}

void kl::Console::SetSize(const UInt2& size) {
	SMALL_RECT consoleRect = { 0, 0, SHORT(size.x - 1), SHORT(size.y - 1) };
	SetConsoleWindowInfo(ConsoleHandle, true, &consoleRect);
}

void kl::Console::SetFont(const UInt2& size, const String& fontName) {
	CONSOLE_FONT_INFOEX cfi = {};
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = SHORT(size.x);
	cfi.dwFontSize.Y = SHORT(size.y);
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, Strings::ToWString(fontName).c_str());
	SetCurrentConsoleFontEx(ConsoleHandle, false, &cfi);
}

char kl::Console::GetInput() {
	char input = 0;
	while (_kbhit()) {
		input = _getch();
	}
	return input;
}

void kl::Console::Wait(char toWaitFor, bool echo) {
	if (echo) {
		if (toWaitFor > 31 && toWaitFor < 127) {
			Print("Press '", toWaitFor, "' to continue...");
		}
		else {
			Print("Press '", int(toWaitFor), "' to continue...");
		}
	}

	char c = 0;
	while ((c = _getch()) != toWaitFor) {
		Print(int(c));
	}
}

char kl::Console::WaitAny(bool echo) {
	if (echo) {
		Print("Press any key to continue...");
	}
	return _getch();
}

static DWORD ignore = 0;
void kl::Console::DumpData(const String& data, const UInt2& location) {
	WriteConsoleOutputCharacterA(ConsoleHandle, data.c_str(), DWORD(data.length()), { short(location.x), short(location.y) }, &ignore);
}

void kl::Console::ProgressBar(const String& message, uint outputY, float percentage) {
	percentage = Math::MinMax(percentage, 0.0f, 1.0f);
	const int barLen = GetSize().x - int(message.length()) - 12;
	const int finishLen = int(barLen * percentage);
	const int emptyLen = barLen - finishLen;

	StringStream stream;
	stream << "  " << message << " [";
	for (int i = 0; i < finishLen; i++) {
		stream << '#';
	}
	for (int i = 0; i < emptyLen; i++) {
		stream << ' ';
	}
	MoveCursor({ 0, outputY });

	printf("%s] %3d%% ", stream.str().c_str(), int(percentage * 100.0f));
}
