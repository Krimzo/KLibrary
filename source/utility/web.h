#pragma once

#include <string>


namespace kl::web {
    std::string download_website(const std::string& url, uint32_t buffer_size = 65536);
}
