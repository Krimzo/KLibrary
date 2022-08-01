#include "window/mouse.h"


kl::mouse::mouse() {
	lmb = { type::LMB };
	mmb = { type::MMB };
	rmb = { type::RMB };
}

kl::mouse::~mouse() {}

void kl::mouse::bind(HWND hwnd) {
	m_Window = hwnd;
}

void kl::mouse::update(int64 type, bool state) {
	for (auto& key : buffer) {
		key.update(type, state);
	}
}

void kl::mouse::update() const {
	for (auto& key : buffer) {
		key.update();
	}
}

bool kl::mouse::hidden() const {
	return m_Hidden;
}

void kl::mouse::hidden(bool enabled) {
	if (!m_Hidden && enabled) {
		while (m_ShowCounter > -1) {
			m_ShowCounter = ShowCursor(false);
		}
	}
	else if (m_Hidden && !enabled) {
		while (m_ShowCounter < 0) {
			m_ShowCounter = ShowCursor(true);
		}
	}
	m_Hidden = enabled;
}

int kl::mouse::scroll() const {
	return m_Scroll;
}

void kl::mouse::scroll(HWND check, int deltaScroll) {
	if (check == m_Window) {
		m_Scroll += deltaScroll;
	}
}

kl::int2 kl::mouse::position(bool client) const {
	if (!client) {
		POINT clientPosition = { m_Position.x, m_Position.y };
		ClientToScreen(m_Window, &clientPosition);
		return { clientPosition.x, clientPosition.y };
	}
	return m_Position;
}

void kl::mouse::position(HWND check, const kl::int2& position) {
	if (check == m_Window) {
		m_Position = position;
	}
}

void kl::mouse::position(const kl::int2& position) {
	if (m_Window) {
		POINT clientPosition = { position.x, position.y };
		ClientToScreen(m_Window, &clientPosition);
		SetCursorPos(clientPosition.x, clientPosition.y);
	}
}

kl::float2 kl::mouse::normalized() const {
	if (m_Window) {
		RECT clientArea = {};
		GetClientRect(m_Window, &clientArea);
		kl::uint2 frameSize = { clientArea.right - clientArea.left, clientArea.bottom - clientArea.top };

		kl::float2 result = {
			float(m_Position.x) / frameSize.x,
			float(frameSize.y - m_Position.y) / frameSize.y
		};
		result *= 2.0f;
		result -= kl::float2(1.0f, 1.0f);
		return result;
	}
	return {};
}
