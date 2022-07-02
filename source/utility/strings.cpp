#include "utility/strings.h"


std::string kl::to::string(const std::wstring& data) {
	std::string temp;
	temp.reserve(data.size());
	for (uint64 i = 0; i < data.size(); i++) {
		if (data[i] < 256) {
			temp.push_back(char(data[i]));
		}
	}
	return temp;
}

std::wstring kl::to::wstring(const std::string& data) {
	std::wstring temp;
	temp.resize(data.size());
	for (uint64 i = 0; i < data.size(); i++) {
		temp[i] = data[i];
	}
	return temp;
}
