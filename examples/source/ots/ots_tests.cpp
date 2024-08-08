#include "examples.h"

namespace ts = kl::ots;


struct Person : public ts::MapSerializable
{
	std::string name;
	float age = 0.0f;

	Person()
	{}

	Person(const std::string& name, float age)
		: name(name), age(age)
	{}

	void to_map(ts::MapContainer& map) const override
	{
		map["name"] = ts::LiteralContainer::make_string(name);
		map["age"] = ts::LiteralContainer::make_float(age);
	}

	void from_map(const ts::MapContainer& map) override
	{
		name = map.at("name")->get_string().value_or("");
		age = map.at("age")->get_float().value_or(0.0f);
	}

	std::string to_string() const
	{
		return kl::format("Person(", name, ", ", age, ")");
	}
};

int examples::ots_tests_main(const int argc, const char** argv)
{
	const auto test = [](const ts::DataContainer& container, const std::string& expected)
	{
		const std::string result = container.to_string(-1);
		kl::assert(result == expected, "Expected: ", expected, ", but got: ", result);
		kl::print("Test passed: ", result);
	};

	// objects
	test(ts::LiteralContainer(""), "null");
	test(ts::LiteralContainer("null"), "null");
	test(ts::LiteralContainer("false"), "false");
	test(ts::LiteralContainer("5"), "5");
	test(ts::LiteralContainer("17.9"), "17.900000");
	test(ts::LiteralContainer("'c'"), "'c'");
	test(ts::LiteralContainer("\"something random $not a comment$\""), "\"something random $not a comment$\"");
	test(ts::LiteralContainer("$this is$ 12 $some comment$"), "12");

	// arrays
	test(ts::ArrayContainer("[]"), "[]");
	test(ts::ArrayContainer("[1, 2, 3, [4, 5, {number:6}, [7, 8, 9 ],], 10]"), "[1, 2, 3, [4, 5, { number: 6 }, [7, 8, 9]], 10]");

	// maps
	test(ts::MapContainer("{}"), "{}");
	test(ts::MapContainer("{data: 16, person: {name: \"Krimzo\", ages: [22,23]}}"), "{ data: 16, person: { name: \"Krimzo\", ages: [22, 23] } }");

	// helpers
	ts::MapContainer map_container{ "{ name: \"Krimzo\", age: 22.0 }" };
	Person person{};
	person.from_container(map_container);
	test(map_container, person.to_container()->to_string(-1));

	kl::print("All tests passed!");
	return 0;
}
