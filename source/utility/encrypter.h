#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>

#include "math/math.h"


namespace kl {
	inline std::wstring toWString(const std::string& data) {
		std::wstring temp;
		temp.resize(data.size());
		for (uint64 i = 0; i < data.size(); i++) {
			temp[i] = data[i];
		}
		return temp;
	}
}

namespace kl {
	class encrypter {
	public:
		using key = std::vector<byte>;
		kl::encrypter::key keys[5] = {};

		encrypter();
		encrypter(const kl::encrypter&) = delete;
		void operator=(const kl::encrypter&) = delete;

		void genKeys();
		void encrypt(void* data, uint64 dataSize) const;
		void decrypt(void* data, uint64 dataSize) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::encrypter& encrypter);
}
