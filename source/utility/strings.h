#pragma once

#include "math/math.h"

#include <string>
#include <sstream>
#include <syncstream>


namespace kl {
	inline constexpr int none = 0;
	inline constexpr char enter = 13;

	namespace to {
		std::string string(const std::wstring& data);
		std::wstring wstring(const std::string& data);
	}

	template<typename... Args> inline std::string format(const Args&... args) {
		std::stringstream ss;
		(ss << ... << args);
		return ss.str();
	}
	template<typename... Args> inline std::wstring wformat(const Args&... args) {
		std::wstringstream ss;
		(ss << ... << args);
		return ss.str();
	}

	template<const char END = '\n', typename... Args> inline void print(const Args&... args) {
		(std::osyncstream(std::cout) << ... << args) << END;
	}
	template<const char END = '\n', typename... Args> inline void wprint(const Args&... args) {
		(std::wosyncstream(std::wcout) << ... << args) << END;
	}
}
