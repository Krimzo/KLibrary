#include "examples.h"

using namespace kl;


class SomeBase
{
public:
	SomeBase() = default;
	virtual ~SomeBase() = default;

    virtual void talk() const
    {
		print("I am SomeBase");
    }
};

static std::ostream& operator<<(std::ostream& stream, const SomeBase& base)
{
    stream << "SomeBase()";
    return stream;
}

class Someone : public SomeBase
{
public:
    const std::string name;

    Someone()
        : name("Unknown")
    {}

    Someone(const std::string& name)
        : name(name)
    {
        print(colors::CYAN, "Someone ", name, " created.", colors::CONSOLE);
    }

    virtual ~Someone()
    {
        print(colors::ORANGE, "Someone ", name, " destroyed.", colors::CONSOLE);
    }

    void talk() const override
    {
        print("I am ", name);
    }
};

static std::ostream& operator<<(std::ostream& stream, const Someone& someone)
{
    stream << "Someone(" << someone.name << ")";
    return stream;
}

void object_test();
void array_test();

int examples::safety_test_main()
{
    object_test();
    array_test();
    return 0;
}

void object_test()
{
    SafeObject<Someone> first_obj = new Someone("First");
    SafeObject<SomeBase> second_obj = new Someone("Second");

    if (first_obj) {
        first_obj->talk();
    }

    second_obj = first_obj;
    if (second_obj) {
		second_obj->talk();
	}

    SafeObject<Someone> third_obj = second_obj.as<Someone>();
    if (third_obj) {
        third_obj->talk();
    }
    
    print(first_obj);  // (0x000002AC6CFB17D0{3}: Someone(First))
    print(second_obj); // (0x000002AC6CFB17D0{3}: SomeBase())
    print(third_obj);  // (0x000002AC6CFB17D0{3}: Someone(First))
}

void array_test()
{
    Array<int> first_array = { 10 };
    for (size_t i = 0; i < first_array.size(); i++) {
        first_array[i] = (int) (i * 2);
    }

    Array<float> second_array = { first_array.size() };
    for (size_t i = 0; i < second_array.size(); i++) {
        second_array[i] = (float) (first_array[i] * first_array[i]);
    }

    print(first_array);  // (0x0000020EFEA41A50{1}: [0, 2, 4, 6, 8, 10, 12, 14, 16, 18])
    print(second_array); // (0x0000020EFEA416D0{1}: [0, 4, 16, 36, 64, 100, 144, 196, 256, 324])
}
