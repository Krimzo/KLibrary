#pragma once

#include <thread>
#include <mutex>
#include <functional>


namespace kl {
	using byte = uint8_t;
	using wchar = wchar_t;
	using uint = uint32_t;
	using int64 = int64_t;
	using uint64 = uint64_t;

	using String = std::string;
	using WString = std::wstring;

	using StringStream = std::stringstream;
	using WStringStream = std::wstringstream;

	using Thread = std::thread;
	using Mutex = std::mutex;

	template<typename T>
	using Vector = std::vector<T>;

	template<typename T>
	using Function = std::function<T>;

	template<typename T>
	using Atomic = std::atomic<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	inline Ref<T> Make(const Args&... args) {
		return std::make_shared<T>(args...);
	}
}
