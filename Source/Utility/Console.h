#pragma once

#include "Graphics/Color.h"
#include "Math/Math.h"


namespace kl {
	void Get();

	bool Warning(bool occured, const String& message, bool wait = false);
	void Assert(bool occured, const String& message, bool wait = true);

	namespace Console {
		void SetEnabled(bool state);
		void Clear();

		void SetCursorEnabled(bool state);
		void MoveCursor(const UInt2& position);

		uint GetWidth();
		void SetWidth(uint width);

		uint GetHeight();
		void SetHeight(uint height);

		UInt2 GetSize();
		void SetSize(const UInt2& size);

		void SetTitle(const String& text);
		void SetFont(const UInt2& size, const String& fontName = "Consolas");

		char GetInput();
		void Wait(char toWaitFor, bool echo = false);
		char WaitAny(bool echo = false);

		void DumpData(const String& data, const UInt2& location = {});

		void ProgressBar(const String& message, uint outputY, float percentage);
	};
}
