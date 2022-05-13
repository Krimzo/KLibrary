#include "utility/encrypter.h"


kl::encrypter::encrypter() {
	std::srand(uint32_t(std::time(nullptr)));
	genKeys();
}

void kl::encrypter::genKeys() {
	for (auto& key : keys) {
		int keySize = 0;
		do {
			keySize = rand() % 7 + 2;
		}
		while ([&]() {
			for (auto& k : keys) {
				if (k.size() == keySize) {
					return true;
				}
			}
			return false;
		}());
		key.resize(keySize);
		for (auto& k : key) {
			k = rand() % 256;
		}
	}
}

void kl::encrypter::printKey(uint32_t ind) {
	if (ind < 5) {
		std::cout << "keys[" << ind << "] = { ";
		for (int k = 0; k < keys[ind].size() - 1; k++) {
			printf("0x%02X, ", keys[ind][k]);
		}
		printf("0x%02X };\n", keys[ind].back());
	}
}
void kl::encrypter::printKeys() {
	for (uint32_t i = 0; i < 5; i++) {
		printKey(i);
	}
}

void kl::encrypter::encrypt(void* data, size_t dataSize) {
	for (auto& key : keys) {
		for (size_t i = 0; i < dataSize; i++) {
			((byte*)data)[i] ^= key[i % key.size()];
		}
	}
}

void kl::encrypter::decrypt(void* data, size_t dataSize) {
	encrypt(data, dataSize);
}
