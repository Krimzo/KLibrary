#pragma once

#include <functional>
#include <thread>


namespace kl::async {
    inline const uint32_t core_count = std::thread::hardware_concurrency();

    void loop(int64_t start_inclusive, int64_t end_exclusive,
              const std::function<void(uint32_t t, int64_t i)>& loop_body, uint32_t thread_count = core_count);
}
