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

int examples::safety_test_main(const int argc, const char** argv)
{
    Ref<Someone> first_obj = new Someone("First");
    Ref<SomeBase> second_obj = new Someone("Second");

    if (first_obj) {
        first_obj->talk();
    }

    second_obj = first_obj;
    if (second_obj) {
        second_obj->talk();
    }

    Ref<Someone> third_obj = second_obj.as<Someone>();
    if (third_obj) {
        third_obj->talk();
    }

    print(first_obj);  // (0x000002AC6CFB17D0{3}: Someone(First))
    print(second_obj); // (0x000002AC6CFB17D0{3}: SomeBase())
    print(third_obj);  // (0x000002AC6CFB17D0{3}: Someone(First))
    return 0;
}
