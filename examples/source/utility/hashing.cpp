#include "examples.h"


int examples::hashing_main()
{
    std::string some_text = "This is a test!";
    kl::float3 some_data = { 1.0f, 2.0f, 3.0f };

    kl::hash_t first_hash = kl::hash(some_text);
    kl::hash_t second_hash = kl::hash(some_data);

    kl::print(first_hash);
    kl::print(second_hash);

    kl::get();
    return 0;
}
