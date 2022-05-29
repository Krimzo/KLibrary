#pragma once

#include "libspec/using.h"
#include "math/int2.h"
#include "graphics/color.h"


namespace kl {
	namespace console {
		void clear();

		void hide();
		void show();

		void setCursor(const kl::int2& position);

		void hideCursor();
		void showCursor();

		void setTitle(const String& text);
		void setFont(const kl::int2& size, const String& fontName = "Consolas");

		kl::int2 getBufferSize();
		kl::int2 getSize();

		void setBufferSize(const kl::int2& size);
		void setSize(const kl::int2& size);

		char getInput();
		void waitFor(char toWaitFor, bool echo = false);
		char waitForAny(bool echo = false);

		void fastOut(const String& data, const kl::int2& location = {});

		void progressBar(const String& message, int outputY, float percentage);
	};
}