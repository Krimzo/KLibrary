#include "utility/thread.h"


// Multithreaded for loop
void kl::thread::parallelFor(int startInclusive, int endExclusive, std::function<void(int, int)> loopBody, int threadCount)
{
	// Thread storage
	std::vector<std::thread> threads(threadCount);

	// Thread creation
	const int runsPerThread = (endExclusive - startInclusive) / threadCount;
	for (int t = 0; t < threadCount; t++)
	{
		const int loopStart = runsPerThread * t + startInclusive;
		const int loopEnd = (t == threadCount - 1) ? endExclusive : (loopStart + runsPerThread);
		threads[t] = std::thread([=](int start, int end)
			{
				for (int i = start; i < end; i++)
					loopBody(t, i);
			}, loopStart, loopEnd);
	}

	// Waiting for the threads to finish
	for (int t = 0; t < threadCount; t++)
		threads[t].join();
}
