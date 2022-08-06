#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <future>

#include "KLTypes.h"


namespace kl::ASync {
	void For(int64 startInclusive, int64 endExclusive, const Function<void(uint t, int64 i)>& loopBody, uint threadCount = Thread::hardware_concurrency());
}
