#include "utility/async.h"


void kl::async::loop(int64 startInclusive, int64 endExclusive, const std::function<void(uint t, int64 i)>& loopBody, uint threadCount)
{
	std::vector<std::thread> workers(threadCount);

	std::atomic<int64> workCounter = startInclusive;
	for (uint t = 0; t < threadCount; t++)
	{
		workers[t] = std::thread([&, t]()
		{
			int64 workIndex = startInclusive;
			do
			{
				workIndex = workCounter++;
				loopBody(t, workIndex);
			}
			while (workIndex < endExclusive);
		});
	}

	for (auto& worker : workers)
	{
		worker.join();
	}
}
