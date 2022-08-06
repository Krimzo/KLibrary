#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>

#include "math/math.h"


namespace kl {
	class Encrypter {
	public:
		using Key = Vector<byte>;

		Key keys[5] = {};

		Encrypter();
		Encrypter(const Encrypter&) = delete;
		void operator=(const Encrypter&) = delete;

		void generateNewKeys();

		void encrypt(void* data, uint64 dataSize) const;
		void decrypt(void* data, uint64 dataSize) const;

		template<typename T>
		void encrypt(T& obj) const {
			encrypt(&obj, sizeof(T));
		}

		template<typename T>
		void decrypt(T& obj) const {
			decrypt(&obj, sizeof(T));
		}
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const Encrypter& encrypter);
}
