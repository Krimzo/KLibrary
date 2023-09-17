#include "examples.h"

using namespace kl;


// Test class
class Person
{
public:
    const std::string name;

    Person()
        : name("Unknown")
    {}

    Person(const std::string& name)
        : name(name)
    {
        print(colors::CYAN, "Person ", name, " created.", colors::CONSOLE);
    }

    virtual ~Person()
    {
        print(colors::ORANGE, "Person ", name, " destroyed.", colors::CONSOLE);
    }

    void talk() const
    {
        print("I am ", name);
    }
};

std::ostream& operator<<(std::ostream& stream, const Person& person)
{
    stream << "Person(" << person.name << ")";
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
    Object<Person> first_object = new Person("First");
    Object<Person> second_object = new Person("Second");

    second_object = first_object;

    if (first_object) first_object->talk();
    if (second_object) second_object->talk();

    print(first_object);  // (0x000002AC6CFB17D0{2}: Person(First))
    print(second_object); // (0x000002AC6CFB17D0{2}: Person(First))
}

void array_test()
{
    Array<int> first_array = { 10 };
    for (uint64_t i = 0; i < first_array.size(); i++) {
        first_array[i] = (int) (i * 2);
    }

    Array<float> second_array = { first_array.size() };
    for (uint64_t i = 0; i < second_array.size(); i++) {
        second_array[i] = (float) (i * i);
    }

    print(first_array);  // (0x0000020EFEA41A50{1}: [0, 2, 4, 6, 8, 10, 12, 14, 16, 18])
    print(second_array); // (0x0000020EFEA416D0{1}: [0, 1, 4, 9, 16, 25, 36, 49, 64, 81])
}
