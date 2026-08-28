#include "klibrary.h"

void kl::Html::apply_var(std::string_view var_name, std::string_view var_value)
{
    replace_all(data, format("${", var_name, "}"), var_value);
}

kl::Html kl::Html::from_string(std::string_view str)
{
    Html result;
    result.data = str;
    return result;
}

kl::Html kl::Html::from_file(std::string_view path)
{
    return from_string(read_file_string(path));
}
