#pragma once


namespace kl {
	typedef unsigned long long key;
	struct keys {
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
		bool key0 = false;
		bool key1 = false;
		bool key2 = false;
		bool key3 = false;
		bool key4 = false;
		bool key5 = false;
		bool key6 = false;
		bool key7 = false;
		bool key8 = false;
		bool key9 = false;

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
		bool up = false;
		bool left = false;
		bool down = false;
		bool right = false;

		// Unknown
		kl::key unknown = NULL;

		// Updates the given key to the given value
		void setKey(kl::key key, bool state) {
			switch (key) {
			// Letters
			case 'Q':
				q = state;
				break;
			case 'W':
				w = state;
				break;
			case 'E':
				e = state;
				break;
			case 'R':
				r = state;
				break;
			case 'T':
				t = state;
				break;
			case 'Z':
				z = state;
				break;
			case 'U':
				u = state;
				break;
			case 'I':
				i = state;
				break;
			case 'O':
				o = state;
				break;
			case 'P':
				p = state;
				break;
			case 'A':
				a = state;
				break;
			case 'S':
				s = state;
				break;
			case 'D':
				d = state;
				break;
			case 'F':
				f = state;
				break;
			case 'G':
				g = state;
				break;
			case 'H':
				h = state;
				break;
			case 'J':
				j = state;
				break;
			case 'K':
				k = state;
				break;
			case 'L':
				l = state;
				break;
			case 'Y':
				y = state;
				break;
			case 'X':
				x = state;
				break;
			case 'C':
				c = state;
				break;
			case 'V':
				v = state;
				break;
			case 'B':
				b = state;
				break;
			case 'N':
				n = state;
				break;
			case 'M':
				m = state;
				break;

			// Numbers
			case '0':
				key0 = state;
				break;
			case '1':
				key1 = state;
				break;
			case '2':
				key2 = state;
				break;
			case '3':
				key3 = state;
				break;
			case '4':
				key4 = state;
				break;
			case '5':
				key5 = state;
				break;
			case '6':
				key6 = state;
				break;
			case '7':
				key7 = state;
				break;
			case '8':
				key8 = state;
				break;
			case '9':
				key9 = state;
				break;

			// Signs
			case VK_OEM_PERIOD:
				period = state;
				break;
			case VK_OEM_COMMA:
				comma = state;
				break;
			case VK_OEM_PLUS:
				plus = state;
				break;
			case VK_OEM_MINUS:
				minus = state;
				break;

			// Special
			case VK_ESCAPE:
				esc = state;
				break;
			case VK_TAB:
				tab = state;
				break;
			case VK_CAPITAL:
				caps = state;
				break;
			case VK_SHIFT:
				shift = state;
				break;
			case VK_CONTROL:
				ctrl = state;
				break;
			case VK_MENU:
				alt = state;
				break;
			case VK_SPACE:
				space = state;
				break;
			case VK_RETURN:
				enter = state;
				break;
			case VK_UP:
				up = state;
				break;
			case VK_LEFT:
				left = state;
				break;
			case VK_DOWN:
				down = state;
				break;
			case VK_RIGHT:
				right = state;
				break;

			default:
				unknown = key;
				break;
			}
		}
	};
}
