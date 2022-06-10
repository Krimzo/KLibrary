#pragma once

#include <windows.h>

#include "math/math.h"


namespace kl {
	namespace time {
		int64 get();
		int64 frequency();
		float calculate(int64 start, int64 end);

		float interval();
		void wait(float seconds);
	};

	class timer {
	private:
		int64 m_IntervalStart;
		int64 m_StopwatchStart;

	public:
		timer();

		float interval();

		void reset();
		float elapsed() const;
	};
}
