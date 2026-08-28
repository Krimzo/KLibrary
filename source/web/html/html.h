#pragma once

#include "apis/apis.h"

namespace kl
{
struct Html
{
    std::string data;

    Html() = default;

    void apply_var(std::string_view var_name, std::string_view var_value);

    static Html from_string(std::string_view str);
    static Html from_file(std::string_view path);
};
} // namespace kl
