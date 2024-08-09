#include "examples.h"

namespace js = kl::json;


struct Person : public js::ObjectSerializable
{
	std::string name;
	float age = 0.0f;

	Person()
	{}

	Person(const std::string& name, float age)
		: name(name), age(age)
	{}

	void to_object(js::Object& object) const override
	{
		object["name"] = js::Literal::make_string(name);
		object["age"] = js::Literal::make_number(age);
	}

	void from_object(const js::Object& object) override
	{
		name = object.at("name")->get_string().value_or("");
		age = object.at("age")->get_float().value_or(0.0f);
	}

	std::string to_string() const
	{
		return kl::format("Person(", name, ", ", age, ")");
	}
};

int examples::json_tests_main(const int argc, const char** argv)
{
	const auto test = [](const js::Container& container, const std::string& expected)
	{
		const std::string result = container.to_string(-1);
		if (result != expected) {
			kl::print("expected: ", expected);
			kl::print(" but got: ", result);
			exit(1);
		}
		kl::print("Test passed: ", result);
	};

	// objects
	test(js::Literal(""), "null");
	test(js::Literal("null"), "null");
	test(js::Literal("false"), "false");
	test(js::Literal("5"), "5.000000");
	test(js::Literal("17.9"), "17.900000");
	test(js::Literal(R"("some "string" in string")"), R"("some "string" in string")");
	test(js::Literal("\"something random $not a comment$\""), "\"something random $not a comment$\"");

	// objects
	test(js::Object("{}"), "{}");
	test(js::Object(R"({ " : some "key" : ": "some "string" in string" })"), R"({ " : some "key" : ": "some "string" in string" })");
	test(js::Object(R"({"data": 16, "person": {"name": "Krimzo", "ages": [22,23]}})"), R"({ "data": 16.000000, "person": { "ages": [22.000000, 23.000000], "name": "Krimzo" } })");

	// arrays
	test(js::Array("[]"), "[]");
	test(js::Array(R"([1, 2, 3, [4, 5, {"number":6}, [7, 8, 9 ],], 10])"), R"([1.000000, 2.000000, 3.000000, [4.000000, 5.000000, { "number": 6.000000 }, [7.000000, 8.000000, 9.000000]], 10.000000])");

	// helpers
	js::Object obj_container{ R"({ "name": "Krimzo", "age": 22.0})" };
	Person person{};
	person.from_container(obj_container);
	test(obj_container, person.to_container()->to_string(-1));

	kl::print("All tests passed!");
	return 0;
}
