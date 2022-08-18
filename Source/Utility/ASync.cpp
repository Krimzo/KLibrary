#include "Utility/ASync.h"


void kl::ASync::For(int64 startInclusive, int64 endExclusive, const Function<void(uint t, int64 i)>& loopBody, uint threadCount) {
	Vector<Thread> workers(threadCount);

	Atomic<int64> workCounter = startInclusive;
	for (uint t = 0; t < threadCount; t++) {
		workers[t] = Thread([&, t]() {
			int64 workIndex = startInclusive;
			do {
				workIndex = workCounter++;
				loopBody(t, workIndex);
			}
			while (workIndex < endExclusive);
		});
	}

	for (auto& worker : workers) {
		worker.join();
	}
}
