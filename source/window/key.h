#pragma once

#include <functional>
#include "math/math.h"


namespace kl {
	class key {
		bool m_State = false;
		int64 m_Type = 0;

	public:
		std::function<void()> press = []() {};
		std::function<void()> down = []() {};
		std::function<void()> release = []() {};

		key(int64 type);
		key(const kl::key&) = delete;
		void operator=(const kl::key&) = delete;
		~key();

		operator bool() const;

		void update(int64 type, bool newState);
		void update() const;
	};
}
