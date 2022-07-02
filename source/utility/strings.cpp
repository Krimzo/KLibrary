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

std::vector<std::string> kl::string::split(const std::string& data, char delimeter) {
	std::vector<std::string> parts;
	std::stringstream dataStream(data);
	for (std::string part; std::getline(dataStream, part, delimeter);) {
		parts.push_back(part);
	}
	return parts;
}

std::vector<std::wstring> kl::string::split(const std::wstring& data, wchar_t delimeter) {
	std::vector<std::wstring> parts;
	std::wstringstream dataStream(data);
	for (std::wstring part; std::getline(dataStream, part, delimeter);) {
		parts.push_back(part);
	}
	return parts;
}
