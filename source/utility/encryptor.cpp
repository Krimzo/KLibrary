#include "utility/encryptor.h"


kl::encryptor::encryptor()
{
    regenerate_keys();
}

void kl::encryptor::regenerate_keys()
{
    for (auto& key : keys) {
        uint32_t key_size = 0;

        do {
            key_size = rand() % 7 + 2;
        }
        while ([&]()
        {
            for (auto& k : keys) {
                if (k.size() == key_size) {
                    return true;
                }
            }
            return false;
        }());

        key.resize(key_size);
        for (auto& k : key) {
            k = static_cast<uint8_t>(rand() % 256);
        }
    }
}

void kl::encryptor::encrypt(void* data, const uint64_t size) const
{
    for (auto& key : keys) {
        for (uint64_t i = 0; i < size; i++) {
            static_cast<uint8_t*>(data)[i] ^= key[i % key.size()];
        }
    }
}

void kl::encryptor::decrypt(void* data, const uint64_t size) const
{
    encrypt(data, size);
}

std::ostream& kl::operator<<(std::ostream& os, const encryptor& encryptor)
{
    for (uint32_t i = 0; i < 5; i++) {
        os << "keys[" << i << "] = { ";
        for (uint32_t k = 0; k < encryptor.keys[i].size() - 1; k++) {
            os << static_cast<uint32_t>(encryptor.keys[i][k]) << ", ";
        }
        os << static_cast<uint32_t>(encryptor.keys[i].back()) << " }" << std::endl;
    }
    return os;
}
