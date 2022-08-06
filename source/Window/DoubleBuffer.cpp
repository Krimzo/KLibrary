#include "Window/DoubleBuffer.h"


kl::DoubleBuffer::DoubleBuffer(const UInt2& frameSize) : m_Buffer1(frameSize), m_Buffer2(frameSize) {
	m_FrontBuffer = &m_Buffer1;
}

kl::UInt2 kl::DoubleBuffer::getSize() const {
	return m_Buffer1.getSize();
}

kl::Image* kl::DoubleBuffer::getBackBuffer() {
	return (m_FrontBuffer == &m_Buffer1) ? &m_Buffer2 : &m_Buffer1;
}

void kl::DoubleBuffer::drawToWindow(Window& window) {
	m_Lock.lock();
	window.drawImage(*m_FrontBuffer);
	m_Lock.unlock();
}

void kl::DoubleBuffer::swapBuffers() {
	m_Lock.lock();
	m_FrontBuffer = getBackBuffer();
	m_Lock.unlock();
}
