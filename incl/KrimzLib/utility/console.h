#pragma once

#include <iostream>
#include <conio.h>

#include "KrimzLib/math/int2.h"
#include "KrimzLib/color/color.h"


namespace kl {
	class console {
	private:
		// Console handle
		static HANDLE handle;

	public:
		// Deleting the def constr
		console() = delete;

		// Clears the console screen
		static void clear();

		// Hides the console
		static void hide();

		// Shows the console
		static void show();

		// Sets the console cursor position
		static void setCursor(const kl::int2& position);

		// Hides the console cursor
		static void hideCursor();

		// Shows the console cursor
		static void showCursor();

		// Sets the console title
		static void setTitle(const std::string& text);

		// Returns screen buffer size
		static kl::int2 getBufferSize();

		// Returns the current console size
		static kl::int2 getSize();

		// Changes the console buffer size
		static void setBufferSize(const kl::int2& size);

		// Changes the console size
		static void setSize(const kl::int2& size);

		// Changes the console font size
		static void setFont(const kl::int2& size, const std::string& fontName = "Consolas");

		// Returns a pressed key
		static char getInput();

		// Waits until the wanted key is pressed
		static void waitFor(char toWaitFor, bool echo = false);

		// Waits for any key to be pressed
		static void waitForAny(bool echo = false);

		// Outputs a progress bar on the console
		static void progressBar(const std::string& message, int outputY, float percentage);

		// Fast console writing
		static void fastOut(const std::string& data, const kl::int2& location = { 0, 0 });
	};
}