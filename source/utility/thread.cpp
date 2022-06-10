#include "utility/thread.h"


void kl::thread::asyncFor(int startInclusive, int endExclusive, const std::function<void(int, int)>& loopBody, int threadCount) {
	std::vector<std::future<void>> futRes(threadCount);
	const int runsPerThread = (endExclusive - startInclusive) / threadCount;
	for (int t = 0; t < threadCount; t++) {
		const int loopStart = runsPerThread * t + startInclusive;
		const int loopEnd = (t == threadCount - 1) ? endExclusive : (loopStart + runsPerThread);
		futRes[t] = std::async(std::launch::async, [&loopBody, t](int start, int end) {
			for (int i = start; i < end; i++) {
				loopBody(t, i);
			}
		}, loopStart, loopEnd);
	}
	for (auto& res : futRes) {
		res.wait();
	}
}
