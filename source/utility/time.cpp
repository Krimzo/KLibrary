#include "utility/time.h"


// Time
int64 kl::time::get() {
	LARGE_INTEGER time = {};
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}
int64 kl::time::frequency() {
	LARGE_INTEGER frequency = {};
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}
float kl::time::calculate(int64 start, int64 end) {
	static const float recFrequency = 1.0f / kl::time::frequency();
	return (end - start) * recFrequency;
}
float kl::time::interval() {
	static int64 startTime = kl::time::get();
	const int64 endTime = kl::time::get();
	const float elapsedTime = kl::time::calculate(startTime, endTime);
	startTime = endTime;
	return elapsedTime;
}
void kl::time::wait(float seconds) {
	const int64 startTime = kl::time::get();
	int64 endTime = kl::time::get();
	while (kl::time::calculate(startTime, endTime) < seconds) {
		endTime = kl::time::get();
	}
}

// Timer
kl::timer::timer() {
	interval();
	reset();
}

float kl::timer::interval() {
	const int64 endTime = kl::time::get();
	const float elapsedTime = kl::time::calculate(m_IntervalStart, endTime);
	m_IntervalStart = endTime;
	return elapsedTime;
}

void kl::timer::reset() {
	m_StopwatchStart = kl::time::get();
}
float kl::timer::elapsed() const {
	return kl::time::calculate(m_StopwatchStart, kl::time::get());
}
