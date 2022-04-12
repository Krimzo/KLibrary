#pragma once

#include <iostream>
#include <windows.h>


namespace kl
{
	class time
	{
	public:
		// Deleting the def constr
		time() = delete;

		// Returns a time since the the last interval() call
		static float interval();

		// Waits for the given time in seconds
		static void wait(float seconds);

	private:
		static LARGE_INTEGER inStartTime;
	};

	class timer
	{
	private:
		LARGE_INTEGER inStartTime;
		LARGE_INTEGER swStartTime;

	public:
		timer();

		// Returns the interval between 2 calls
		float interval();

		// Resets the last time
		void reset();

		// Returns the passed time since the last reset
		float elapsed() const;
	};
}
