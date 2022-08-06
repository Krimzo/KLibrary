#pragma once

#include <windows.h>

#include "Math/Math.h"
#include "Window/Key.h"


namespace kl {
	class Mouse {
		HWND m_Window = nullptr;
		Int2 m_Position = {};
		bool m_Hidden = false;
		int m_ShowCounter = 0;
		int m_Scroll = 0;

	public:
		enum Type {
			LMB = 0,
			MMB,
			RMB
		};

		union {
			struct {
				kl::Key left;
				kl::Key middle;
				kl::Key right;
			};
			kl::Key buffer[3];
		};

		Mouse();
		Mouse(const Mouse&) = delete;
		void operator=(const Mouse&) = delete;
		~Mouse();

		void bindToWindow(HWND hwnd);

		void updateValue(int64 type, bool state);

		void updateCall() const;

		bool isHidden() const;
		void setHidden(bool enabled);

		int getScroll() const;
		void updateScroll(HWND check, int deltaScroll);

		Int2 getPosition(bool client = true) const;
		void updatePosition(HWND check, const Int2& position);
		void setPosition(const kl::Int2& position);

		Float2 getNormalizedPosition() const;
	};
}
