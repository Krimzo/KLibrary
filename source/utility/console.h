#pragma once

#include "graphics/color.h"
#include "math/math.h"


namespace kl
{
	namespace console
	{
		void hide();
		void show();
		void clear();

		void hideCursor();
		void showCursor();
		void moveCursor(const kl::uint2& pos);

		kl::uint2 size();
		void resize(const kl::uint2& size);

		void title(const std::string& text);
		void font(const kl::uint2& size, const std::string& fontName = "Consolas");

		char input();
		void waitFor(char toWaitFor, bool echo = false);
		char waitForAny(bool echo = false);

		bool warning(bool occured, const std::string& message, bool wait = false);
		void error(bool occured, const std::string& message, bool wait = true);

		void dump(const std::string& data, const kl::uint2& location = {});

		void bar(const std::string& message, uint outputY, float percentage);
	};
}