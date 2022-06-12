#include "window/key.h"


kl::key::operator bool() {
	return m_State;
}

void kl::key::update(bool newState) {
	if (!m_State && newState) {
		press();
	}
	else if (m_State && !newState) {
		release();
	}
	m_State = newState;
}

bool kl::key::update() const {
	if (m_State) {
		down();
		return true;
	}
	return false;
}
