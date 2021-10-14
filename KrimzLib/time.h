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
			QueryPerformanceCounter(&stopwatchLast);
		}

		// Loads the current pc frequency
		static void LoadPCFrequency()
		{
			LARGE_INTEGER counterTempFreq;
			QueryPerformanceFrequency(&counterTempFreq);
			PCFrequency = double(counterTempFreq.QuadPart);
			StaticGetElapsed();
			StaticStopwatchReset();
		}

		// Returns a time since the the last StaticGetElapsed() call
		static double StaticGetElapsed()
		{
			static LARGE_INTEGER staticCounterNow;
			QueryPerformanceCounter(&staticCounterNow);
			double time = (staticCounterNow.QuadPart - staticCounterLast.QuadPart) / PCFrequency;
			staticCounterLast = staticCounterNow;
			return time;
		}

		// Resets the static stopwatch to 0 seconds
		static void StaticStopwatchReset()
		{
			QueryPerformanceCounter(&staticStopwatchLast);
		}

		// Returns the passed time since the last static stopwatch reset
		static double StaticStopwatchElapsed()
		{
			static LARGE_INTEGER staticStopwatchNow;
			QueryPerformanceCounter(&staticStopwatchNow);
			return (staticStopwatchNow.QuadPart - staticStopwatchLast.QuadPart) / PCFrequency;
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

		// Resets the stopwatch to 0 seconds
		void StopwatchReset()
		{
			QueryPerformanceCounter(&stopwatchLast);
		}

		// Returns the passed time since the last stopwatch reset
		double StopwatchElapsed()
		{
			static LARGE_INTEGER stopwatchNow;
			QueryPerformanceCounter(&stopwatchNow);
			return (stopwatchNow.QuadPart - stopwatchLast.QuadPart) / PCFrequency;
		}

		// Returns a time since the the last GetElapsed() call
		double GetElapsed()
		{
			static LARGE_INTEGER counterNow;
			QueryPerformanceCounter(&counterNow);
			double time = (counterNow.QuadPart - counterLast.QuadPart) / PCFrequency;
			counterLast = counterNow;
			return time;
		}

	private:
		static double PCFrequency;
		static LARGE_INTEGER staticCounterLast;
		static LARGE_INTEGER staticStopwatchLast;
		LARGE_INTEGER counterLast = {};
		LARGE_INTEGER stopwatchLast = {};
	};
	double time::PCFrequency = 0;
	LARGE_INTEGER time::staticCounterLast = {};
	LARGE_INTEGER time::staticStopwatchLast = {};
}
