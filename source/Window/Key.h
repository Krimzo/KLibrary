#pragma once

#include "Math/Math.h"
#include "Utility/ASync.h"


namespace kl {
	class Key {
		bool m_State = false;
		int64 m_Type = 0;

	public:
		Function<void()> onPress = []() {};
		Function<void()> onDown = []() {};
		Function<void()> onRelease = []() {};

		Key(int64 type);
		Key(const Key&) = delete;
		void operator=(const Key&) = delete;
		~Key();

		operator bool() const;

		void updateValue(int64 type, bool newState);

		void updateCall() const;
	};
}
