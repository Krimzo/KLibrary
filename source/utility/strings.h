#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <syncstream>

#include "KLTypes.h"


namespace kl {
	namespace Strings {
		String ToString(const WString& data);
		WString ToWString(const String& data);

		Vector<String> Split(const String& data, char delimeter);
		Vector<WString> WSplit(const WString& data, wchar delimeter);
	}

	// 8 bit chars
	template<bool NewLine = true, typename... Args>
	inline void Write(std::ostream& stream, const Args&... args) {
		std::osyncstream syncedStream(stream);
		(syncedStream << ... << args);
		if constexpr (NewLine) {
			syncedStream << std::endl;
		}
	}

	template<bool NewLine = true, typename... Args>
	inline void Print(const Args&... args) {
		Write<NewLine>(std::cout, args...);
	}

	template<typename... Args>
	inline String Format(const Args&... args) {
		StringStream stream;
		Write<false>(stream, args...);
		return stream.str();
	}

	// 16 bit chars
	template<bool NewLine = true, typename... Args>
	inline void WWrite(std::wostream& wstream, const Args&... args) {
		std::wosyncstream wsyncedStream(wstream);
		(wsyncedStream << ... << args);
		if constexpr (NewLine) {
			wsyncedStream << std::endl;
		}
	}

	template<bool NewLine = true, typename... Args>
	inline void WPrint(const Args&... args) {
		WWrite<NewLine>(std::wcout, args...);
	}

	template<typename... Args>
	inline WString WFormat(const Args&... args) {
		WStringStream wstream;
		WWrite<false>(wstream, args...);
		return wstream.str();
	}
}
