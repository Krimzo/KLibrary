#pragma once

#include <windows.h>

#include "math/math.h"


namespace kl
{
	namespace time
	{
		int64 get();
		int64 frequency();
		float calculate(int64 start, int64 end);

		float interval();
		void wait(float seconds);
		bool sleep(float seconds);

		struct date
		{
			int year;
			int month;
			int day;
			int hour;
			int minute;
			int second;

			date()
			{
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

		std::ostream& operator<<(std::ostream& stream, const date& date);
	}

	class timer
	{
		int64 m_IntervalStart;
		int64 m_IntervalEnd;
		int64 m_ElapsedStart;

	public:
		timer();

		void reset();

		void newInterval();

		template<typename T = float>
		T interval() const
		{
			return T(kl::time::calculate(m_IntervalStart, m_IntervalEnd));
		}

		void newElapsed();

		template<typename T = float>
		T elapsed() const
		{
			return T(kl::time::calculate(m_ElapsedStart, kl::time::get()));
		}
	};
}
