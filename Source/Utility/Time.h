#pragma once

#include <windows.h>

#include "Math/Math.h"


namespace kl::Time {
	int64 Get();
	int64 CPUFrequency();
	float Calculate(int64 start, int64 end);

	float GetInterval();
	void Wait(float seconds);
	bool Sleep(float seconds);
}

namespace kl {
	struct Date {
		int year, month, day, hour, minute, second;

		Date() {
			int64 time = std::time(nullptr);
			std::tm now = {};
			localtime_s(&now, &time);

			year = now.tm_year + 1900;
			month = now.tm_mon + 1;
			day = now.tm_mday;
			hour = now.tm_hour;
			minute = now.tm_min;
			second = now.tm_sec;
		}
	};

	std::ostream& operator<<(std::ostream& stream, const Date& date);
}

namespace kl {
	class Timer {
		int64 m_IntervalStart;
		int64 m_IntervalEnd;
		int64 m_ElapsedStart;

	public:
		Timer();

		void reset();

		void updateInterval();
		float getInterval() const;

		void updateElapsed();
		float getElapsed() const;
	};
}
