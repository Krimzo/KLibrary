#include "klibrary.h"


std::string kl::convert_string( std::wstring_view const& data )
{
    std::string temp;
    temp.resize( data.size() );
    for ( size_t i = 0; i < data.size(); i++ )
        temp[i] = char( data[i] );

    return temp;
}

std::wstring kl::convert_string( std::string_view const& data )
{
    std::wstring temp;
    temp.resize( data.size() );
    for ( size_t i = 0; i < data.size(); i++ )
        temp[i] = wchar_t( data[i] );

    return temp;
}

std::vector<std::string> kl::split_string( std::string_view const& data, std::string_view const& delimiter )
{
    std::vector<std::string> parts;
    for ( auto const& part : std::views::split( data, delimiter ) )
        parts.emplace_back( part.begin(), part.end() );
    return parts;
}

std::vector<std::wstring> kl::split_string( std::wstring_view const& data, std::wstring_view const& delimiter )
{
    std::vector<std::wstring> parts;
    for ( auto const& part : std::views::split( data, delimiter ) )
        parts.emplace_back( part.begin(), part.end() );
    return parts;
}

void kl::replace_all( std::string& str, std::string_view const& from, std::string_view const& to )
{
    if ( str.empty() || from.empty() )
        return;

    size_t index = 0;
    while ( ( index = str.find( from, index ) ) != -1 )
    {
        str.replace( index, from.length(), to );
        index += to.length();
    }
}

void kl::replace_all( std::wstring& str, std::wstring_view const& from, std::wstring_view const& to )
{
    if ( str.empty() || from.empty() )
        return;

    size_t index = 0;
    while ( ( index = str.find( from, index ) ) != -1 )
    {
        str.replace( index, from.length(), to );
        index += to.length();
    }
}

std::optional<int64_t> kl::parse_int( std::string_view const& data )
{
    if ( data.empty() )
        return std::nullopt;

    char* last_char = nullptr;
    int64_t result = std::strtoll( data.data(), &last_char, 10 );
    if ( last_char != data.data() + data.size() )
        return std::nullopt;

    return { result };
}

std::optional<double> kl::parse_float( std::string_view const& data )
{
    if ( data.empty() )
        return std::nullopt;

    char* last_char = nullptr;
    double result = std::strtod( data.data(), &last_char );
    if ( last_char != data.data() + data.size() )
        return std::nullopt;

    return { result };
}
