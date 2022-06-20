#pragma once

#include <windows.h>

#include "math/math.h"
#include "window/keys.h"


namespace kl {
	class mouse {
	private:
		HWND m_Win = nullptr;
		int m_ShowCounter = 0;

	public:
		kl::key lmb;
		kl::key mmb;
		kl::key rmb;
		kl::int2 position;
		int scroll = 0;

		void bind(HWND hwnd);

		void update() const;

		void hide();
		void show();
		void move(const kl::int2& pos);

		kl::float2 normalize(const kl::uint2& frameSize) const;
	};
}
