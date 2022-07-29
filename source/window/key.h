#pragma once

#include <functional>


namespace kl
{
	class key
	{
	private:
		bool m_State = false;

	public:
		std::function<void()> press = []()
		{};
		std::function<void()> down = []()
		{};
		std::function<void()> release = []()
		{};

		operator bool();

		void update(bool newState);
		bool update() const;
	};
}
