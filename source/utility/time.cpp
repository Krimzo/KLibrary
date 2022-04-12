#include "utility/time.h"


namespace _ignore
{
	// Getting the pc frequency
	static const float recFreq = []
	{
		LARGE_INTEGER tempFreq = {};
		QueryPerformanceFrequency(&tempFreq);
		return 1.0f / tempFreq.QuadPart;
	}();

	// Calculates elapsed time from 2 given LARGE_INTEGERS
	float CalcTime(const LARGE_INTEGER& startTime, const LARGE_INTEGER& endTime)
	{
		return (endTime.QuadPart - startTime.QuadPart) * recFreq;
	}
}

/* TIME */
// Static interval
LARGE_INTEGER kl::time::inStartTime = []
{
	LARGE_INTEGER currentTime = {};
	QueryPerformanceCounter(&currentTime);
	return currentTime;
}();

// Returns a time since the the last interval() call
float kl::time::interval()
{
	LARGE_INTEGER inEndTime = {};
	QueryPerformanceCounter(&inEndTime);
	const float elapsedTime = _ignore::CalcTime(inStartTime, inEndTime);
	inStartTime = inEndTime;
	return elapsedTime;
}

// Waits for the given time in seconds
void kl::time::wait(float seconds)
{
	LARGE_INTEGER sleepStartT = {}, sleepEndT = {};
	QueryPerformanceCounter(&sleepStartT);
	do
	{
		QueryPerformanceCounter(&sleepEndT);
	}
	while (_ignore::CalcTime(sleepStartT, sleepEndT) < seconds);
}

/* TIMER */
kl::timer::timer()
{
	interval();
	reset();
}

// Returns the interval between 2 calls
float kl::timer::interval()
{
	LARGE_INTEGER inEndTime = {};
	QueryPerformanceCounter(&inEndTime);
	const float elapsedTime = _ignore::CalcTime(inStartTime, inEndTime);
	inStartTime = inEndTime;
	return elapsedTime;
}

// Resets the last stopwatch time
void kl::timer::reset()
{
	QueryPerformanceCounter(&swStartTime);
}

// Returns the passed time since the last reset
float kl::timer::elapsed() const
{
	LARGE_INTEGER swEndTime = {};
	QueryPerformanceCounter(&swEndTime);
	return _ignore::CalcTime(swStartTime, swEndTime);
}
