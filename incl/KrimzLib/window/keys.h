#pragma once


namespace kl {
	typedef unsigned long long key;
	class keys {
	public:
		// Letters
		bool q = false;
		bool w = false;
		bool e = false;
		bool r = false;
		bool t = false;
		bool z = false;
		bool u = false;
		bool i = false;
		bool o = false;
		bool p = false;
		bool a = false;
		bool s = false;
		bool d = false;
		bool f = false;
		bool g = false;
		bool h = false;
		bool j = false;
		bool k = false;
		bool l = false;
		bool y = false;
		bool x = false;
		bool c = false;
		bool v = false;
		bool b = false;
		bool n = false;
		bool m = false;

		// Numbers
		bool num0 = false;
		bool num1 = false;
		bool num2 = false;
		bool num3 = false;
		bool num4 = false;
		bool num5 = false;
		bool num6 = false;
		bool num7 = false;
		bool num8 = false;
		bool num9 = false;

		// Signs
		bool period = false;
		bool comma = false;
		bool plus = false;
		bool minus = false;

		// Special
		bool esc = false;
		bool tab = false;
		bool caps = false;
		bool shift = false;
		bool ctrl = false;
		bool alt = false;
		bool space = false;
		bool enter = false;
		bool insert = false;
		bool delet = false;
		bool up = false;
		bool left = false;
		bool down = false;
		bool right = false;

		// Function
		bool f1 = false;
		bool f2 = false;
		bool f3 = false;
		bool f4 = false;
		bool f5 = false;
		bool f6 = false;
		bool f7 = false;
		bool f8 = false;
		bool f9 = false;
		bool f10 = false;
		bool f11 = false;
		bool f12 = false;

		// Unknown
		kl::key unknown = 0;

		// Updates the given key to the given value
		void setKey(kl::key key, bool state);
	};
}
