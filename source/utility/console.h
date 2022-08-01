#pragma once

#include "graphics/color.h"
#include "math/math.h"


namespace kl {
	void get();

	namespace console {
		void hide();
		void show();
		void clear();

		void cursor(bool enable);
		void cursor(const kl::uint2& position);

		kl::uint2 size();
		void size(const kl::uint2& size);

		void title(const std::string& text);
		void font(const kl::uint2& size, const std::string& fontName = "Consolas");

		char input();
		void wait(char toWaitFor, bool echo = false);
		char waitAny(bool echo = false);

		bool warning(bool occured, const std::string& message, bool wait = false);
		void error(bool occured, const std::string& message, bool wait = true);

		void dump(const std::string& data, const kl::uint2& location = {});

		void bar(const std::string& message, uint outputY, float percentage);
	};
}