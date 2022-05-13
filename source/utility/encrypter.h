#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>


namespace kl {
	class encrypter {
	public:
		using key = std::vector<byte>;
		kl::encrypter::key keys[5] = {};

		encrypter();
		encrypter(const kl::encrypter&) = delete;
		void operator=(const kl::encrypter&) = delete;

		void genKeys();
		void printKey(uint32_t ind);
		void printKeys();
		void encrypt(void* data, size_t dataSize);
		void decrypt(void* data, size_t dataSize);
	};
}
