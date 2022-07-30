#include "utility/time.h"


// Time
int64 kl::time::get()
{
	LARGE_INTEGER time = {};
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

int64 kl::time::frequency()
{
	LARGE_INTEGER frequency = {};
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}

float kl::time::calculate(int64 start, int64 end)
{
	static const float recFrequency = 1.0f / kl::time::frequency();
	return (end - start) * recFrequency;
}

float kl::time::interval()
{
	static int64 startTime = kl::time::get();
	const int64 endTime = kl::time::get();
	const float elapsedTime = kl::time::calculate(startTime, endTime);
	startTime = endTime;
	return elapsedTime;
}

void kl::time::wait(float seconds)
{
	const int64 startTime = kl::time::get();
	int64 endTime = kl::time::get();
	while (kl::time::calculate(startTime, endTime) < seconds)
	{
		endTime = kl::time::get();
	}
}

bool kl::time::sleep(float seconds)
{
	HANDLE timer = CreateWaitableTimerA(nullptr, true, nullptr);
	if (!timer)
	{
		return false;
	}

	static const int64 frequency = kl::time::frequency();
	const int64 toSleep = -int64(seconds * frequency);
	if (!SetWaitableTimer(timer, (LARGE_INTEGER*) &toSleep, 0, nullptr, nullptr, false))
	{
		CloseHandle(timer);
		return false;
	}

	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
	return true;
}

std::ostream& kl::time::operator<<(std::ostream& stream, const kl::time::date& date)
{
	stream << date.year <<
		'.' << date.month <<
		'.' << date.day <<
		' ' << date.hour <<
		':' << date.minute <<
		':' << date.second;
	return stream;
}

// Timer
kl::timer::timer()
{
	int64 now = kl::time::get();
	m_IntervalStart = now;
	m_IntervalEnd = now;
	m_ElapsedStart = now;
}

void kl::timer::reset()
{
	newInterval();
	newElapsed();
}

void kl::timer::newInterval()
{
	m_IntervalStart = m_IntervalEnd;
	m_IntervalEnd = kl::time::get();
}

void kl::timer::newElapsed()
{
	m_ElapsedStart = kl::time::get();
}
