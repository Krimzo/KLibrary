#include "klibrary.h"


kl::json::Array::Array()
{}

kl::json::Array::Array(const std::initializer_list<Ref<Container>>& items)
{
    insert(begin(), items.begin(), items.end());
}

kl::json::Array::Array(const std::string& data)
{
    const auto tokens = Lexer::parse(data);
    compile(tokens.begin(), tokens.end());
}

bool kl::json::Array::compile(std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last)
{
    if (first == last) {
        return false;
    }

    int depth = 0;
    for (auto it = first; it != last; ++it) {
        if (depth == 1) {
            Ref<Container> container;
            if (it->type == TokenType::_ARRAY_START) {
                container = new Array();
            }
            else if (it->type == TokenType::_OBJECT_START) {
                container = new Object();
            }
            else {
                container = new Literal();
            }
            if (container->compile(it, last)) {
                push_back(container);
            }
        }
        if (it->type == TokenType::_OBJECT_START || it->type == TokenType::_ARRAY_START) {
            depth += 1;
        }
        else if (it->type == TokenType::_OBJECT_END || it->type == TokenType::_ARRAY_END) {
            depth -= 1;
            if (depth <= 0) {
                break;
            }
        }
    }
    return true;
}

std::string kl::json::Array::decompile(const int depth) const
{
    if (empty()) {
        return format(Standard::array_start_literal, Standard::array_end_literal);
    }

    std::stringstream stream;
    stream << Standard::array_start_literal;
    for (size_t i = 0; i < size(); i++) {
        stream << at(i)->decompile(-1);
        if (i < size() - 1) {
            stream << Standard::splitter_literal << ' ';
        }
    }
    stream << Standard::array_end_literal;
    return stream.str();
}