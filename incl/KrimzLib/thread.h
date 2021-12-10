#pragma once


namespace kl {
	namespace thread {
		// Multithreaded for loop
		void parallelFor(int startInclusive, int endExclusive, int threadCount, std::function<void(int i)> loopBody) {
			// Thread storage
			std::vector<std::thread> cpuThreads(threadCount);
			
			// Thread creation
			int runsPerThread = (endExclusive - startInclusive) / threadCount;
			for (int i = 0; i < threadCount; i++) {
				int loopStart = runsPerThread * i + startInclusive;
				int loopEnd = (i == threadCount - 1) ? endExclusive : (loopStart + runsPerThread);
				cpuThreads[i] = std::thread([&](int start, int end) { for (int i = start; i < end; i++) { loopBody(i); } }, loopStart, loopEnd);
			}

			// Waiting for the threads to finish
			for (int i = 0; i < threadCount; i++) {
				cpuThreads[i].join();
			}
		}
	};
}
