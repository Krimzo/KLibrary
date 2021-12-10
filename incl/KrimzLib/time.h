#pragma once


namespace kl {
	class time {
	public:
		// Returns a time since the the last staticGetElapsed() call
		static double staticGetElapsed() {
			LARGE_INTEGER staticCounterNow;
			QueryPerformanceCounter(&staticCounterNow);
			double elapsedTime = (staticCounterNow.QuadPart - staticCounterLast.QuadPart) / PCFrequency;
			staticCounterLast = staticCounterNow;
			return elapsedTime;
		}

		// Resets the static stopwatch to 0 seconds
		static void staticStopwatchReset() {
			QueryPerformanceCounter(&staticStopwatchLast);
		}

		// Returns the passed time since the last static stopwatch reset
		static double staticStopwatchElapsed() {
			LARGE_INTEGER staticStopwatchNow;
			QueryPerformanceCounter(&staticStopwatchNow);
			return (staticStopwatchNow.QuadPart - staticStopwatchLast.QuadPart) / PCFrequency;
		}

		// Waits for the given time in seconds
		static void wait(double seconds) {
			LARGE_INTEGER sleepCounterStart = {}, sleepCounter;
			QueryPerformanceCounter(&sleepCounterStart);
			do {
				QueryPerformanceCounter(&sleepCounter);
			} while (((sleepCounter.QuadPart - sleepCounterStart.QuadPart) / PCFrequency) < seconds);
		}

		// Constructor
		time() {
			QueryPerformanceCounter(&counterLast);
			QueryPerformanceCounter(&stopwatchLast);
		}

		// Returns a time since the the last getElapsed() call
		double getElapsed() {
			LARGE_INTEGER counterNow;
			QueryPerformanceCounter(&counterNow);
			double elapsedTime = (counterNow.QuadPart - counterLast.QuadPart) / PCFrequency;
			counterLast = counterNow;
			return elapsedTime;
		}

		// Resets the stopwatch to 0 seconds
		void stopwatchReset() {
			QueryPerformanceCounter(&stopwatchLast);
		}

		// Returns the passed time since the last stopwatch reset
		double stopwatchElapsed() {
			LARGE_INTEGER stopwatchNow;
			QueryPerformanceCounter(&stopwatchNow);
			return (stopwatchNow.QuadPart - stopwatchLast.QuadPart) / PCFrequency;
		}

	protected:
		// Loads the current pc frequency
		static void loadPCFrequency() {
			LARGE_INTEGER counterTempFreq;
			QueryPerformanceFrequency(&counterTempFreq);
			PCFrequency = double(counterTempFreq.QuadPart);
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