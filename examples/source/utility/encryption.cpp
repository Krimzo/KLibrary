#include "examples.h"


int examples::encryption_main()
{
    const kl::Encryptor encryptor = {};

    auto data = kl::Float4x4::scaling({ 1.0f, 2.0f, 3.0f });
    print(kl::colors::SKY, data);

    encryptor.encrypt(data);
    print(kl::colors::ORANGE, data);

    encryptor.decrypt(data);
    print(kl::colors::CYAN, data);

    kl::get();
    return 0;
}
