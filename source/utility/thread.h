#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <future>


namespace kl {
	namespace thread {
		void asyncFor(int startInclusive, int endExclusive, const std::function<void(int t, int i)>& loopBody, int threadCount = 4);
	}
}
