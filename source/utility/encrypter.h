#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>

#include "math/math.h"


namespace kl
{
	class encrypter
	{
	public:
		using key = std::vector<byte>;
		kl::encrypter::key keys[5] = {};

		encrypter();
		encrypter(const kl::encrypter&) = delete;
		void operator=(const kl::encrypter&) = delete;

		void genKeys();
		void encrypt(void* data, uint64 dataSize) const;
		void decrypt(void* data, uint64 dataSize) const;

		template<typename T>
		void encrypt(T& obj) const
		{
			encrypt(&obj, sizeof(T));
		}

		template<typename T>
		void decrypt(T& obj) const
		{
			decrypt(&obj, sizeof(T));
		}
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::encrypter& encrypter);
}
