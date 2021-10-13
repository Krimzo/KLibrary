#pragma once
#include <windows.h>


namespace kl
{
	class time
	{
	public:
		// Constructor
		time()
		{
			QueryPerformanceCounter(&counterLast);
		}

		// Loads the current pc frequency
		static void LoadPCFrequency()
		{
			LARGE_INTEGER counterTempFreq;
			QueryPerformanceFrequency(&counterTempFreq);
			PCFrequency = double(counterTempFreq.QuadPart);
		}

		// Returns a time since the the last static GetElapsed call
		static double GetStaticElapsed()
		{
			static LARGE_INTEGER counterStaticNow, counterStaticLast;
			QueryPerformanceCounter(&counterStaticNow);
			double time = (counterStaticNow.QuadPart - counterStaticLast.QuadPart) / PCFrequency;
			counterStaticLast = counterStaticNow;
			return time;
		}

		// Returns a time since the the last GetElapsed call
		double GetElapsed()
		{
			QueryPerformanceCounter(&counterNow);
			double time = (counterNow.QuadPart - counterLast.QuadPart) / PCFrequency;
			counterLast = counterNow;
			return time;
		}

		// Sleeps for the given time
		static void WaitSeconds(double time)
		{
			static LARGE_INTEGER sleepCounterStart, sleepCounter;
			QueryPerformanceCounter(&sleepCounterStart);
			do {
				QueryPerformanceCounter(&sleepCounter);
			} while (((sleepCounter.QuadPart - sleepCounterStart.QuadPart) / PCFrequency) < time);
		}
		static void WaitMiliseconds(double time)
		{
			time /= 1000;
			static LARGE_INTEGER sleepCounterStart, sleepCounter;
			QueryPerformanceCounter(&sleepCounterStart);
			do {
				QueryPerformanceCounter(&sleepCounter);
			} while (((sleepCounter.QuadPart - sleepCounterStart.QuadPart) / PCFrequency) < time);
		}

	private:
		static double PCFrequency;
		LARGE_INTEGER counterNow = {};
		LARGE_INTEGER counterLast = {};
	};
	double time::PCFrequency = 0;
}
