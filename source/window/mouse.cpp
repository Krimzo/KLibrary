#include "window/mouse.h"


void kl::mouse::bind(HWND hwnd) {
	m_Win = hwnd;
}

void kl::mouse::update() const {
	lmb.update();
	mmb.update();
	rmb.update();
}

void kl::mouse::hide() {
	while (m_ShowCounter > -1) {
		m_ShowCounter = ShowCursor(false);
	}
}
void kl::mouse::show() {
	while (m_ShowCounter < 0) {
		m_ShowCounter = ShowCursor(true);
	}
}

void kl::mouse::move(const kl::int2& pos) {
	POINT clientPos = { pos.x, pos.y };
	ClientToScreen(m_Win, &clientPos);
	SetCursorPos(clientPos.x, clientPos.y);
}

kl::float2 kl::mouse::normalize(const kl::uint2& frameSize) const {
	kl::float2 result = {
		float(position.x) / frameSize.x,
		float(frameSize.y - position.y) / frameSize.y
	};
	result *= 2.0f;
	result -= kl::float2(1.0f, 1.0f);
	return result;
}
