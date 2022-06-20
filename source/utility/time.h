#pragma once

#include <windows.h>

#include "math/math.h"


namespace kl {
	namespace time {
		int64 get();
		int64 frequency();
		double calculate(int64 start, int64 end);

		double interval();
		void wait(double seconds);
		bool sleep(double seconds);
	};

	class timer {
	private:
		int64 m_IntervalStart;
		int64 m_StopwatchStart;

	public:
		timer();

		double interval();

		void reset();
		double elapsed() const;
	};
}
