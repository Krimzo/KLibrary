#pragma once

#include "libspec/using.h"
#include "graphics/color.h"
#include "math/math.h"


namespace kl {
	namespace console {
		void hide();
		void show();
		void clear();

		void hideCursor();
		void showCursor();
		void moveCursor(const kl::int2& pos);

		kl::int2 size();
		void resize(const kl::int2& size);

		void setTitle(const String& text);
		void setFont(const kl::int2& size, const String& fontName = "Consolas");

		char input();
		void waitFor(char toWaitFor, bool echo = false);
		char waitForAny(bool echo = false);

		void fastOut(const String& data, const kl::int2& location = {});

		void progressBar(const String& message, int outputY, float percentage);
	};
}