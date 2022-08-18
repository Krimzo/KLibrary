#include "Window/Mouse.h"


kl::Mouse::Mouse() :
	left(Type::LMB),
	middle(Type::MMB),
	right(Type::RMB) {}

kl::Mouse::~Mouse() {}

void kl::Mouse::bindToWindow(HWND hwnd) {
	m_Window = hwnd;
}

void kl::Mouse::updateValue(int64 type, bool state) {
	for (auto& key : buffer) {
		key.updateValue(type, state);
	}
}

void kl::Mouse::updateCall() const {
	for (auto& key : buffer) {
		key.updateCall();
	}
}

bool kl::Mouse::isHidden() const {
	return m_Hidden;
}

void kl::Mouse::setHidden(bool enabled) {
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

int kl::Mouse::getScroll() const {
	return m_Scroll;
}

void kl::Mouse::updateScroll(HWND check, int deltaScroll) {
	if (check == m_Window) {
		m_Scroll += deltaScroll;
	}
}

kl::Int2 kl::Mouse::getPosition(bool client) const {
	if (!client) {
		POINT clientPosition = { m_Position.x, m_Position.y };
		ClientToScreen(m_Window, &clientPosition);
		return { clientPosition.x, clientPosition.y };
	}
	return m_Position;
}

void kl::Mouse::updatePosition(HWND check, const Int2& position) {
	if (check == m_Window) {
		m_Position = position;
	}
}

void kl::Mouse::setPosition(const Int2& position) {
	if (m_Window) {
		POINT clientPosition = { position.x, position.y };
		ClientToScreen(m_Window, &clientPosition);
		SetCursorPos(clientPosition.x, clientPosition.y);
	}
}

kl::Float2 kl::Mouse::getNormalizedPosition() const {
	if (m_Window) {
		RECT clientArea = {};
		GetClientRect(m_Window, &clientArea);
		UInt2 frameSize = { clientArea.right - clientArea.left, clientArea.bottom - clientArea.top };

		Float2 result = {
			float(m_Position.x) / frameSize.x,
			float(frameSize.y - m_Position.y) / frameSize.y
		};
		result *= 2.0f;
		result -= { 1.0f, 1.0f };
		return result;
	}
	return {};
}
