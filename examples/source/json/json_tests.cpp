#include "examples.h"

namespace js = kl::json;


struct Person : js::ObjectSerializable
{
    std::string name;
    float age = 0.0f;

    Person()
    {}

    Person( std::string const& name, float age )
        : name( name ), age( age )
    {}

    void to_object( js::Object& object ) override
    {
        object["name"] = js::make_string( name );
        object["age"] = js::make_number( age );
    }

    void from_object( js::Object const& object ) override
    {
        name = object.at( "name" )->get_string().value_or( "" );
        age = object.at( "age" )->get_float().value_or( 0.0f );
    }

    std::string to_string() const
    {
        return kl::format( "Person(", name, ", ", age, ")" );
    }
};

int examples::json_tests_main( int argc, char** argv )
{
    auto test = []( js::Container const& container, std::string const& expected )
    {
        std::string result = container.decompile( -1 );
        if ( result != expected )
        {
            kl::print( "expected: ", expected );
            kl::print( " but got: ", result );
            exit( 1 );
        }
        kl::print( "Test passed: ", result );
    };

    test( js::Literal( "" ), "null" );
    test( js::Literal( "null" ), "null" );
    test( js::Literal( "false" ), "false" );
    test( js::Literal( "5" ), "5" );
    test( js::Literal( "17.9" ), "17.9" );
    test( js::Literal( R"("some \"string\" \\ in \t string")" ), R"("some \"string\" \\ in \t string")" );
    test( js::Literal( R"("something random $not a comment$")" ), R"("something random $not a comment$")" );

    test( js::Object( "{}" ), "{}" );
    test( js::Object( R"({ " : some \": key\" : ": "some \"string\" in string" })" ), R"({ " : some \": key\" : ": "some \"string\" in string" })" );
    test( js::Object( R"({"data": 16, "person": {"name": "Krimzo", "ages": [22,23]}})" ), R"({ "data": 16, "person": { "ages": [22, 23], "name": "Krimzo" } })" );

    test( js::Array( "[]" ), "[]" );
    test( js::Array( R"([1, 2, 3, [4, 5, {"number":6}, [7, 8, 9 ],], 10])" ), R"([1, 2, 3, [4, 5, { "number": 6 }, [7, 8, 9]], 10])" );

    js::Object obj_container{ R"({ "name": "Krimzo", "age": 22.0})" };
    Person person{};
    person.from_container( obj_container );
    test( obj_container, person.to_container()->decompile( -1 ) );

    kl::print( "All tests passed!" );
    return 0;
}
