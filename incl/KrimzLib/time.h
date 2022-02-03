#pragma once


namespace kl {
	namespace _kl_dont_use_ {
		static float recPCFrequency = 0;
		struct _kl_time_initialization_ {
			_kl_time_initialization_() {
				LARGE_INTEGER tempFreq = {};
				QueryPerformanceFrequency(&tempFreq);
				recPCFrequency = 1.0f / tempFreq.QuadPart;
			}
		};
		_kl_time_initialization_ _time_init_;

		// Calculates elapsed time from 2 given LARGE_INTEGERS
		float CalcElapTime(const LARGE_INTEGER& startTime, const LARGE_INTEGER& endTime) {
			return (endTime.QuadPart - startTime.QuadPart) * _kl_dont_use_::recPCFrequency;
		}
	}

	class time {
	public:
		// Returns a time since the the last interval() call
		static float interval() {
			LARGE_INTEGER inEndTime = {};
			QueryPerformanceCounter(&inEndTime);
			const float elapsedTime = _kl_dont_use_::CalcElapTime(inStartTime, inEndTime);
			inStartTime = inEndTime;
			return elapsedTime;
		}

		// Waits for the given time in seconds
		static void wait(float seconds) {
			LARGE_INTEGER sleepStartT = {}, sleepEndT = {};
			QueryPerformanceCounter(&sleepStartT);
			do {
				QueryPerformanceCounter(&sleepEndT);
			} while (_kl_dont_use_::CalcElapTime(sleepStartT, sleepEndT) < seconds);
		}

	private:
		static LARGE_INTEGER inStartTime;
	};
	LARGE_INTEGER time::inStartTime = {};

	struct timer {
		// Constructor
		timer() {
			interval();
			reset();
		}

		// Returns the interval between 2 calls
		float interval() {
			LARGE_INTEGER inEndTime = {};
			QueryPerformanceCounter(&inEndTime);
			const float elapsedTime = _kl_dont_use_::CalcElapTime(inStartTime, inEndTime);
			inStartTime = inEndTime;
			return elapsedTime;
		}

		// Resets the last time
		void reset() {
			QueryPerformanceCounter(&swStartTime);
		}

		// Returns the passed time since the last reset
		float elapsed() {
			LARGE_INTEGER swEndTime = {};
			QueryPerformanceCounter(&swEndTime);
			return _kl_dont_use_::CalcElapTime(swStartTime, swEndTime);
		}

	private:
		LARGE_INTEGER inStartTime = {};
		LARGE_INTEGER swStartTime = {};
	};
}