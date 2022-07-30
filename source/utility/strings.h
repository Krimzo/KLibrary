#pragma once

#include "math/math.h"

#include <vector>
#include <string>
#include <sstream>
#include <syncstream>


namespace kl {
	using wchar = wchar_t;

	inline constexpr int none = 0;
	inline constexpr char enter = 13;
	inline constexpr char space = 32;

	namespace to {
		std::string string(const std::wstring& data);
		std::wstring wstring(const std::string& data);
	}

	namespace string {
		std::vector<std::string> split(const std::string& data, char delimeter);
		std::vector<std::wstring> split(const std::wstring& data, wchar_t delimeter);
	}

	template<const char END = '\n', typename Arg0, typename... Args>
	inline void print(const Arg0& arg0, const Args&... args) {
		if constexpr (std::derived_from<Arg0, std::ostream>) {
			(std::osyncstream((std::ostream&) arg0) << ... << args) << END;
		}
		else {
			((std::osyncstream(std::cout) << arg0) << ... << args) << END;
		}
	}

	template<const wchar END = L'\n', typename Arg0, typename... Args>
	inline void wprint(const Arg0& arg0, const Args&... args) {
		if constexpr (std::derived_from<Arg0, std::wostream>) {
			(std::wosyncstream((std::wostream&) arg0) << ... << args) << END;
		}
		else {
			((std::wosyncstream(std::wcout) << arg0) << ... << args) << END;
		}
	}

	template<typename... Args>
	inline std::string format(const Args&... args) {
		std::stringstream ss;
		kl::print(ss, args...);
		return ss.str();
	}

	template<typename... Args>
	inline std::wstring wformat(const Args&... args) {
		std::wstringstream ss;
		kl::wprint(ss, args...);
		return ss.str();
	}
}
