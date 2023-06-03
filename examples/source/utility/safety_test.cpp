#include "klib.h"

using namespace kl;


// Test class
struct person
{
    const std::string name;

    person()
        : person("unknown")
    {}

    person(const std::string& name)
        : name(name)
    {
        print(colors::cyan, "Person ", name, " created.", colors::console);
    }

    ~person()
    {
        print(colors::orange, "Person ", name, " destroyed.", colors::console);
    }

    void talk() const
    {
        print("I am ", name);
    }
};

std::ostream& operator<<(std::ostream& stream, const person& person)
{
    stream << "Person(" << person.name << ")";
    return stream;
}

void object_test();
void array_test();

int main()
{
    object_test();
    array_test();
}

void object_test()
{
    object<person> first_object = new person("First");
    object<person> second_object = new person("Second");

    second_object = first_object;

    if (first_object) first_object->talk();
    if (second_object) second_object->talk();

    print(first_object);  // (0x000002AC6CFB17D0{2}: Person(First))
    print(second_object); // (0x000002AC6CFB17D0{2}: Person(First))
}

void array_test()
{
    array<int> first_array = { 10 };
    for (uint64_t i = 0; i < first_array.size(); i++) {
        first_array[i] = (int) (i * 2);
    }

    array<float> second_array = { first_array.size() };
    for (uint64_t i = 0; i < second_array.size(); i++) {
        second_array[i] = (float) (i * i);
    }

    print(first_array);  // (0x0000020EFEA41A50{1}: [0, 2, 4, 6, 8, 10, 12, 14, 16, 18])
    print(second_array); // (0x0000020EFEA416D0{1}: [0, 1, 4, 9, 16, 25, 36, 49, 64, 81])
}
