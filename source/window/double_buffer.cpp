#include "window/double_buffer.h"


kl::double_buffer::double_buffer(const kl::uint2& frameSize) : m_Buffer1(frameSize), m_Buffer2(frameSize) {
	m_FrontBuffer = &m_Buffer1;
}

kl::uint2 kl::double_buffer::size() {
	return m_Buffer1.size();
}

kl::image* kl::double_buffer::getBB() {
	return (m_FrontBuffer == &m_Buffer1) ? &m_Buffer2 : &m_Buffer1;
}

void kl::double_buffer::draw(kl::window& win) {
	m_Lock.lock();
	win.draw(*m_FrontBuffer);
	m_Lock.unlock();
}
void kl::double_buffer::operator>>(kl::window& win) {
	draw(win);
}

void kl::double_buffer::swap() {
	m_Lock.lock();
	m_FrontBuffer = getBB();
	m_Lock.unlock();
}
