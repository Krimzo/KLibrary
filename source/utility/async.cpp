#include "utility/async.h"


void kl::async::loop(const int64_t start_inclusive, const int64_t end_exclusive,
                     const std::function<void(uint32_t t, int64_t i)>& loop_body, const uint32_t thread_count)
{
    std::vector<std::thread> workers(thread_count);
    std::atomic work_counter = start_inclusive;

    for (uint32_t t = 0; t < thread_count; t++) {
        workers[t] = std::thread([&work_counter, end_exclusive, &loop_body, t] {
            for (int64_t work_index = work_counter++; work_index < end_exclusive; work_index = work_counter++) {
                loop_body(t, work_index);
            }
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }
}
