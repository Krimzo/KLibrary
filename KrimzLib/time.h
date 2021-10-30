#pragma once


namespace kl {
	class time {
	public:
		// Constructor
		time() {
			counterLast = std::chrono::high_resolution_clock::now();
			stopwatchLast = counterLast;
		}

		// Returns a time since the the last StaticGetElapsed() call
		static double StaticGetElapsed() {
			auto staticCounterNow = std::chrono::high_resolution_clock::now();
			double elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(staticCounterNow - staticCounterLast).count() / 1000000000.0;
			staticCounterLast = staticCounterNow;
			return elapsedTime;
		}

		// Resets the static stopwatch to 0 seconds
		static void StaticStopwatchReset() {
			staticStopwatchLast = std::chrono::high_resolution_clock::now();
		}

		// Returns the passed time since the last static stopwatch reset
		static double StaticStopwatchElapsed() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - staticStopwatchLast).count() / 1000000000.0;
		}

		// Resets the stopwatch to 0 seconds
		void StopwatchReset() {
			stopwatchLast = std::chrono::high_resolution_clock::now();
		}

		// Returns the passed time since the last stopwatch reset
		double StopwatchElapsed() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - stopwatchLast).count() / 1000000000.0;
		}

		// Returns a time since the the last GetElapsed() call
		double GetElapsed() {
			auto counterNow = std::chrono::high_resolution_clock::now();
			double elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(counterNow - counterLast).count() / 1000000000.0;
			counterLast = counterNow;
			return elapsedTime;
		}

		// Sleeps for the given time
		static void Wait(double seconds) {
			auto sleepCounterStart = std::chrono::high_resolution_clock::now();
			while(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - sleepCounterStart).count() / 1000000000.0 < seconds);
		}

	private:
		static std::chrono::_V2::system_clock::time_point staticCounterLast;
		static std::chrono::_V2::system_clock::time_point staticStopwatchLast;
		std::chrono::_V2::system_clock::time_point counterLast = {};
		std::chrono::_V2::system_clock::time_point stopwatchLast = {};
	};
	std::chrono::_V2::system_clock::time_point time::staticCounterLast = {};
	std::chrono::_V2::system_clock::time_point time::staticStopwatchLast = {};
}
