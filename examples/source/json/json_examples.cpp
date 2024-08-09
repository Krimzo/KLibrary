#include "examples.h"

namespace js = kl::json;


struct FirstData : public js::ObjectSerializable
{
    std::string id;
    int value = 0;

    void to_object(js::Object& object) const override
    {
        object["id"] = js::Literal::make_string(id);
        object["value"] = js::Literal::make_number(value);
    }

    void from_object(const js::Object& object) override
    {
        id = object.at("id")->get_string().value_or("");
        value = object.at("value")->get_int().value_or(0);
    }

    std::string to_string() const
    {
        return kl::format("FirstData(", id, ", ", value, ")");
    }
};

struct SecondData : public js::ObjectSerializable
{
    std::string id;
    FirstData first_data = {};
    float chance = 0.0f;

    void to_object(js::Object& object) const override
    {
        object["id"] = js::Literal::make_string(id);
        object["first_data"] = first_data.to_container();
        object["chance"] = js::Literal::make_number(chance);
    }

    void from_object(const js::Object& object) override
    {
        id = object.at("id")->get_string().value_or("");
        first_data.from_container(*object.at("first_data"));
        chance = object.at("chance")->get_float().value_or(0.0);
    }

    std::string to_string() const
    {
        return kl::format("SecondData(", id, ", ", first_data.to_string(), ", ", chance, ")");
    }
};

int examples::json_examples_main(const int argc, const char** argv)
{
    // string -> object
    SecondData second_data{};
    second_data.from_container(js::Object(R"(
{
    "id": "some_id_1",
    "first_data": {
        "id": "some_id_0",
        "value": 16
    },
    "chance": 0.5
}
    )"));
    kl::print(second_data.to_string());

    // object -> string
    kl::Ref container = second_data.to_container();
    kl::print(container->decompile());
    return 0;
}
