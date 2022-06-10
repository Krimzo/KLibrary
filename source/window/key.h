#pragma once

#include <functional>


namespace kl {
	class key {
	private:
		bool m_State;

	public:
		std::function<void()> press;
		std::function<void()> down;
		std::function<void()> release;

		key();

		operator bool();

		void update(bool newState);
		bool update() const;
	};
}
