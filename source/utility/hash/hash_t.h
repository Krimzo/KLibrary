#pragma once

#include "apis/apis.h"

namespace kl
{
struct Hash
{
    uint8_t buffer[32] = {};

    Hash();
    Hash(std::string_view hash);

    uint8_t& operator[](size_t index);
    uint8_t const& operator[](size_t index) const;

    bool operator==(Hash const& other) const;
    bool operator!=(Hash const& other) const;
};
} // namespace kl

namespace kl
{
std::ostream& operator<<(std::ostream& stream, Hash const& hash);
}
