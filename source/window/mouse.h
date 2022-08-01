#pragma once

#include <windows.h>

#include "math/math.h"
#include "window/keys.h"


namespace kl {
	class mouse {
		HWND m_Window = nullptr;
		kl::int2 m_Position = {};
		bool m_Hidden = false;
		int m_ShowCounter = 0;
		int m_Scroll = 0;

	public:
		enum type {
			LMB = 0,
			MMB,
			RMB
		};

		union {
			struct {
				kl::key lmb;
				kl::key mmb;
				kl::key rmb;
			};
			kl::key buffer[3];
		};

		mouse();
		~mouse();

		void bind(HWND hwnd);

		void update(int64 type, bool state);
		void update() const;

		bool hidden() const;
		void hidden(bool enabled);

		int scroll() const;
		void scroll(HWND check, int deltaScroll);

		kl::int2 position(bool client = true) const;
		void position(HWND check, const kl::int2& position);
		void position(const kl::int2& position);

		kl::float2 normalized() const;
	};
}
