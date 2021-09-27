#pragma once
#include <windows.h>


namespace kl {
	class time {
	public:
		// Loads the current pc frequency
		static void LoadPCFrequency() {
			QueryPerformanceFrequency(&counterLast);
			PCFrequency = double(counterLast.QuadPart);
		}

		// Returns a time since the the last GetElapsed call
		static double GetElapsed() {
			QueryPerformanceCounter(&counterNow);
			double time = (counterNow.QuadPart - counterLast.QuadPart) / PCFrequency;
			counterLast = counterNow;
			return time;
		}

	private:
		static LARGE_INTEGER counterNow;
		static LARGE_INTEGER counterLast;
		static double PCFrequency;
	};
	LARGE_INTEGER time::counterNow = {};
	LARGE_INTEGER time::counterLast = {};
	double time::PCFrequency = 0;
}
