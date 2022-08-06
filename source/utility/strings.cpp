#include "Utility/Strings.h"
#include "Math/Math.h"


kl::String kl::Strings::ToString(const WString& data) {
	String temp;
	temp.reserve(data.size());
	for (uint64 i = 0; i < data.size(); i++) {
		if (data[i] < 256) {
			temp.push_back(char(data[i]));
		}
	}
	return temp;
}

kl::WString kl::Strings::ToWString(const String& data) {
	WString temp;
	temp.resize(data.size());
	for (uint64 i = 0; i < data.size(); i++) {
		temp[i] = data[i];
	}
	return temp;
}

kl::Vector<kl::String> kl::Strings::Split(const String& data, char delimeter) {
	Vector<String> parts;
	StringStream dataStream(data);
	for (String part; std::getline(dataStream, part, delimeter);) {
		parts.push_back(part);
	}
	return parts;
}

kl::Vector<kl::WString> kl::Strings::WSplit(const WString& data, wchar_t delimeter) {
	Vector<WString> parts;
	WStringStream dataStream(data);
	for (WString part; std::getline(dataStream, part, delimeter);) {
		parts.push_back(part);
	}
	return parts;
}
