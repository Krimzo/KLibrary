#include "utility/async.h"


void kl::async::loop(int64 startInclusive, int64 endExclusive, const std::function<void(uint t, int64 i)>& loopBody, uint threadCount) {
	std::vector<std::thread> workers(threadCount);

	const int64 runsPerThread = (endExclusive - startInclusive) / threadCount;
	for (uint t = 0; t < threadCount; t++) {
		const int64 loopStart = runsPerThread * t + startInclusive;
		const int64 loopEnd = (t == threadCount - 1) ? endExclusive : (loopStart + runsPerThread);
		workers[t] = std::thread([&loopBody, t, loopStart, loopEnd]() {
			for (int64 i = loopStart; i < loopEnd; i++) {
				loopBody(t, i);
			}
		});
	}

	for (auto& worker : workers) {
		worker.join();
	}
}
