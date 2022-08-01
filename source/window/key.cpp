#include "window/key.h"


kl::key::key() : m_Type(NULL) {}

kl::key::key(int type) : m_Type(type) {}

kl::key::~key() {}

kl::key::operator bool() const {
	return m_State;
}

void kl::key::update(int64 type, bool newState) {
	if (type == m_Type) {
		if (!m_State && newState) {
			press();
		}
		else if (m_State && !newState) {
			release();
		}
		m_State = newState;
	}
}

void kl::key::update() const {
	if (m_State) {
		down();
	}
}
