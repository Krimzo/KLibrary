#pragma once

#include <functional>
#include "math/math.h"


namespace kl {
	class key {
	private:
		bool m_State = false;
		int64 m_Type = NULL;

	public:
		std::function<void()> press = []() {};
		std::function<void()> down = []() {};
		std::function<void()> release = []() {};

		key();
		key(int type);
		~key();

		operator bool() const;

		void update(int64 type, bool newState);
		void update() const;
	};
}
