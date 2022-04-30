#include "window/double_buffer.h"


kl::double_buffer::double_buffer(const kl::int2& frameSize) : buff1(frameSize), buff2(frameSize) {
	fb = &buff1;
}

kl::int2 kl::double_buffer::size() {
	return buff1.size();
}

kl::image* kl::double_buffer::getBB() {
	return (fb == &buff1) ? &buff2 : &buff1;
}

void kl::double_buffer::draw(kl::window& win) {
	lock.lock();
	win.drawImage(*fb);
	lock.unlock();
}
void kl::double_buffer::operator>>(kl::window& win) {
	draw(win);
}

void kl::double_buffer::swap() {
	lock.lock();
	fb = getBB();
	lock.unlock();
}
