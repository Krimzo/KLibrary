#include "window/key.h"


kl::Key::Key(int64 type) : m_Type(type) {}

kl::Key::~Key() {}

kl::Key::operator bool() const {
	return m_State;
}

void kl::Key::updateValue(int64 type, bool newState) {
	if (type == m_Type) {
		if (!m_State && newState) {
			onPress();
		}
		else if (m_State && !newState) {
			onRelease();
		}
		m_State = newState;
	}
}

void kl::Key::updateCall() const {
	if (m_State) {
		onDown();
	}
}
