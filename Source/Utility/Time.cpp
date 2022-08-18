#include "Utility/Time.h"


/* Time */
kl::int64 kl::Time::Get() {
	LARGE_INTEGER time = {};
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

kl::int64 kl::Time::CPUFrequency() {
	LARGE_INTEGER frequency = {};
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}

float kl::Time::Calculate(int64 start, int64 end) {
	static const float recFrequency = 1.0f / Time::CPUFrequency();
	return (end - start) * recFrequency;
}

float kl::Time::GetInterval() {
	static int64 startTime = Time::Get();
	const int64 endTime = Time::Get();
	const float elapsedTime = Time::Calculate(startTime, endTime);
	startTime = endTime;
	return elapsedTime;
}

void kl::Time::Wait(float seconds) {
	const int64 startTime = Time::Get();
	while (Time::Calculate(startTime, Time::Get()) < seconds);
}

bool kl::Time::Sleep(float seconds) {
	HANDLE timer = CreateWaitableTimerA(nullptr, true, nullptr);
	if (!timer) {
		return false;
	}

	static const int64 frequency = Time::CPUFrequency();
	const int64 toSleep = -int64(seconds * frequency);
	if (!SetWaitableTimer(timer, (LARGE_INTEGER*) &toSleep, 0, nullptr, nullptr, false)) {
		CloseHandle(timer);
		return false;
	}

	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
	return true;
}

/* Date */
std::ostream& kl::operator<<(std::ostream& stream, const Date& date) {
	stream << date.year <<
		'.' << date.month <<
		'.' << date.day <<
		' ' << date.hour <<
		':' << date.minute <<
		':' << date.second;
	return stream;
}

/* Timer */
kl::Timer::Timer() {
	int64 now = Time::Get();
	m_IntervalStart = now;
	m_IntervalEnd = now;
	m_ElapsedStart = now;
}

void kl::Timer::reset() {
	updateInterval();
	updateElapsed();
}

void kl::Timer::updateInterval() {
	m_IntervalStart = m_IntervalEnd;
	m_IntervalEnd = Time::Get();
}

float kl::Timer::getInterval() const {
	return Time::Calculate(m_IntervalStart, m_IntervalEnd);
}

void kl::Timer::updateElapsed() {
	m_ElapsedStart = Time::Get();
}

float kl::Timer::getElapsed() const {
	return Time::Calculate(m_ElapsedStart, Time::Get());
}
