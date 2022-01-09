#pragma once


namespace kl {
	namespace _kl_dont_use_ {
		float PCFrequency = 0;
		struct _kl_time_initialization_ {
			_kl_time_initialization_() {
				LARGE_INTEGER counterTempFreq;
				QueryPerformanceFrequency(&counterTempFreq);
				PCFrequency = float(counterTempFreq.QuadPart);
			}
		};
		_kl_time_initialization_ _time_init_;
	}

	static class time {
	public:
		// Returns a time since the the last staticGetElapsed() call
		static float getInterval() {
			LARGE_INTEGER intervalNow;
			QueryPerformanceCounter(&intervalNow);
			float elapsedTime = (intervalNow.QuadPart - intervalLast.QuadPart) / _kl_dont_use_::PCFrequency;
			intervalLast = intervalNow;
			return elapsedTime;
		}

		// Waits for the given time in seconds
		static void wait(float seconds) {
			LARGE_INTEGER sleepCounterStart = {}, sleepCounter = {};
			QueryPerformanceCounter(&sleepCounterStart);
			do {
				QueryPerformanceCounter(&sleepCounter);
			} while (((sleepCounter.QuadPart - sleepCounterStart.QuadPart) / _kl_dont_use_::PCFrequency) < seconds);
		}

	private:
		static LARGE_INTEGER intervalLast;
	};
	LARGE_INTEGER time::intervalLast = {};

	struct timer {
		// Constructor
		timer() {
			reset();
		}

		// Resets the stopwatch and the interval getter
		void reset() {
			QueryPerformanceCounter(&intervalLast);
			QueryPerformanceCounter(&stopwatchLast);
		}

		// Returns a time since the the last getElapsed() call
		float interval() {
			LARGE_INTEGER intervalNow;
			QueryPerformanceCounter(&intervalNow);
			float elapsedTime = (intervalNow.QuadPart - intervalLast.QuadPart) / _kl_dont_use_::PCFrequency;
			intervalLast = intervalNow;
			return elapsedTime;
		}

		// Resets the stopwatch to 0 seconds
		void swReset() {
			QueryPerformanceCounter(&stopwatchLast);
		}

		// Returns the passed time since the last stopwatch reset
		float swElapsed() {
			LARGE_INTEGER stopwatchNow;
			QueryPerformanceCounter(&stopwatchNow);
			return (stopwatchNow.QuadPart - stopwatchLast.QuadPart) / _kl_dont_use_::PCFrequency;
		}

	private:
		LARGE_INTEGER intervalLast = {};
		LARGE_INTEGER stopwatchLast = {};
	};
}