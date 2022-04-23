#include "window/key.h"


kl::key::key() : state(false), press([]() {}), down([]() {}), release([]() {}) {}

kl::key::operator bool() {
	return state;
}

void kl::key::update(bool newState) {
	if (!state && newState) {
		press();
	}
	else if (state && !newState) {
		release();
	}
	state = newState;
}
