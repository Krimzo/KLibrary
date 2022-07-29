#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <future>

#include "math/math.h"


namespace kl
{
	namespace async
	{
		void loop(int64 startInclusive, int64 endExclusive, const std::function<void(uint t, int64 i)>& loopBody, uint threadCount = std::thread::hardware_concurrency());
	}
}
