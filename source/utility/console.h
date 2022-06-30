#pragma once

#include "graphics/color.h"
#include "math/math.h"


namespace kl {
	inline const int NONE = 0;
	template<const char END = '\n', typename... Args> inline void print(const Args&... args) {
		(std::cout << ... << args) << END;
	}

	namespace console {
		void hide();
		void show();
		void clear();

		void hideCursor();
		void showCursor();
		void moveCursor(const kl::uint2& pos);

		kl::uint2 size();
		void resize(const kl::uint2& size);

		void setTitle(const std::string& text);
		void setFont(const kl::uint2& size, const std::string& fontName = "Consolas");

		char input();
		void waitFor(char toWaitFor, bool echo = false);
		char waitForAny(bool echo = false);

		bool warning(bool occured, const std::string& message, bool wait = false);
		void error(bool occured, const std::string& message, bool wait = true);

		void dump(const std::string& data, const kl::uint2& location = {});

		void bar(const std::string& message, uint outputY, float percentage);
	};
}