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
			QueryPerformanceFrequency(&counterStaticLast);
			PCFrequency = double(counterStaticLast.QuadPart);
			QueryPerformanceCounter(&counterStaticLast);
		}

		// Returns a time since the the last static GetElapsed call
		static double GetStaticElapsed()
		{
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

	private:
		static LARGE_INTEGER counterStaticNow;
		static LARGE_INTEGER counterStaticLast;
		static double PCFrequency;
		LARGE_INTEGER counterNow = {};
		LARGE_INTEGER counterLast = {};
	};
	LARGE_INTEGER time::counterStaticNow = {};
	LARGE_INTEGER time::counterStaticLast = {};
	double time::PCFrequency = 0;
}
