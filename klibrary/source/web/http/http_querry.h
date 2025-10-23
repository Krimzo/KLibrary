#pragma once

#include "web/http/http_request.h"


namespace kl
{
std::string encode_url_string( std::string_view const& data );
std::string decode_url_string( std::string_view const& data );

struct HttpQuerry : std::map<std::string, std::string>
{
    std::string path;

    HttpQuerry() = default;
    HttpQuerry( std::string_view const& full_path );
};
}
