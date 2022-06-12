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
double kl::time::calculate(int64 start, int64 end) {
	static const double recFrequency = 1.0 / kl::time::frequency();
	return (end - start) * recFrequency;
}
double kl::time::interval() {
	static int64 startTime = kl::time::get();
	const int64 endTime = kl::time::get();
	const double elapsedTime = kl::time::calculate(startTime, endTime);
	startTime = endTime;
	return elapsedTime;
}
void kl::time::wait(double seconds) {
	const int64 startTime = kl::time::get();
	int64 endTime = kl::time::get();
	while (kl::time::calculate(startTime, endTime) < seconds) {
		endTime = kl::time::get();
	}
}
bool kl::time::sleep(double seconds) {
	HANDLE timer = CreateWaitableTimerA(nullptr, true, nullptr);
	if (!timer) {
		return false;
	}

	static const int64 frequency = kl::time::frequency();
	const int64 toSleep = -int64(seconds * frequency);
	if (!SetWaitableTimer(timer, (LARGE_INTEGER*)&toSleep, 0, nullptr, nullptr, false)) {
		CloseHandle(timer);
		return false;
	}

	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
	return true;
}

// Timer
kl::timer::timer() {
	interval();
	reset();
}

double kl::timer::interval() {
	const int64 endTime = kl::time::get();
	const double elapsedTime = kl::time::calculate(m_IntervalStart, endTime);
	m_IntervalStart = endTime;
	return elapsedTime;
}

void kl::timer::reset() {
	m_StopwatchStart = kl::time::get();
}
double kl::timer::elapsed() const {
	return kl::time::calculate(m_StopwatchStart, kl::time::get());
}
