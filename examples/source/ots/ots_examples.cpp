#include "examples.h"

namespace ts = kl::ots;


struct FirstData : public ts::MapSerializable
{
    std::string id;
    int value = 0;

    void to_map(ts::MapContainer& map) const override
    {
        map["id"] = ts::LiteralContainer::load_string(id);
        map["value"] = ts::LiteralContainer::load_int(value);
    }

    void from_map(const ts::MapContainer& map) override
    {
        id = map.at("id")->get_string().value_or("");
        value = map.at("value")->get_int().value_or(0);
    }

    std::string to_string() const
    {
        return kl::format("FirstData(", id, ", ", value, ")");
    }
};

struct SecondData : public ts::MapSerializable
{
    std::string id;
    FirstData first_data = {};
    float chance = 0.0f;

    void to_map(ts::MapContainer& map) const override
    {
        map["id"] = ts::LiteralContainer::load_string(id);
        map["first_data"] = first_data.to_container();
        map["chance"] = ts::LiteralContainer::load_float(chance);
    }

    void from_map(const ts::MapContainer& map) override
    {
        id = map.at("id")->get_string().value_or("");
        first_data.from_container(*map.at("first_data"));
        chance = map.at("chance")->get_float().value_or(0.0);
    }

    std::string to_string() const
    {
        return kl::format("SecondData(", id, ", ", first_data.to_string(), ", ", chance, ")");
    }
};

int examples::ots_examples_main()
{
    // From string
    SecondData second_data{};
    second_data.from_container(ts::MapContainer(R"(
        {
            id: "some_id_1",
            first_data: {
                id: "some_id_0",
                value: 16
            },
            chance: 0.5
        }
    )"));
    kl::print(second_data.to_string()); // SecondData(some_id_1, FirstData(some_id_0, 16), 0.5)

    // To string
    kl::Object container = second_data.to_container();
    kl::print(container->to_string()); // { id: "some_id_1", first_data: { id: "some_id_0", value: 16 }, chance: 0.500000 }
    return 0;
}
