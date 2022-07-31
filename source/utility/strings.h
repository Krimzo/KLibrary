#pragma once

#include "math/math.h"

#include <vector>
#include <string>
#include <sstream>
#include <syncstream>


namespace kl {
	using wchar = wchar_t;

	namespace to {
		std::string string(const std::wstring& data);
		std::wstring wstring(const std::string& data);
	}

	namespace string {
		std::vector<std::string> split(const std::string& data, char delimeter);
		std::vector<std::wstring> split(const std::wstring& data, wchar_t delimeter);
	}

	// 8 bit chars
	template<bool NewLine = true, typename... Args>
	inline void write(std::ostream& stream, const Args&... args) {
		std::osyncstream syncedStream(stream);
		(syncedStream << ... << args);
		if constexpr (NewLine) {
			syncedStream << std::endl;
		}
	}

	template<bool NewLine = true, typename... Args>
	inline void print(const Args&... args) {
		kl::write<NewLine>(std::cout, args...);
	}

	template<typename... Args>
	inline std::string format(const Args&... args) {
		std::stringstream stream;
		kl::write<false>(stream, args...);
		return stream.str();
	}

	// 16 bit chars
	template<bool NewLine = true, typename... Args>
	inline void wwrite(std::wostream& wstream, const Args&... args) {
		std::wosyncstream wsyncedStream(wstream);
		(wsyncedStream << ... << args);
		if constexpr (NewLine) {
			wsyncedStream << std::endl;
		}
	}

	template<bool NewLine = true, typename... Args>
	inline void wprint(const Args&... args) {
		kl::wwrite<NewLine>(std::wcout, args...);
	}

	template<typename... Args>
	inline std::wstring wformat(const Args&... args) {
		std::wstringstream wstream;
		kl::wwrite<false>(wstream, args...);
		return wstream.str();
	}
}
