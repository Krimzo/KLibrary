#pragma once


namespace kl {
	struct keys {
		// Letters
		bool q;
		bool w;
		bool e;
		bool r;
		bool t;
		bool z;
		bool u;
		bool i;
		bool o;
		bool p;
		bool a;
		bool s;
		bool d;
		bool f;
		bool g;
		bool h;
		bool j;
		bool k;
		bool l;
		bool y;
		bool x;
		bool c;
		bool v;
		bool b;
		bool n;
		bool m;

		// Numbers
		bool key0;
		bool key1;
		bool key2;
		bool key3;
		bool key4;
		bool key5;
		bool key6;
		bool key7;
		bool key8;
		bool key9;

		// Signs
		bool period;
		bool comma;
		bool plus;
		bool minus;

		// Special
		bool esc;
		bool tab;
		bool caps;
		bool shift;
		bool ctrl;
		bool alt;
		bool space;
		bool enter;
		bool up;
		bool left;
		bool down;
		bool right;

		// Unknown
		kl::key unknown;

		// Constructor
		keys() {
			// Letters
			q = false;
			w = false;
			e = false;
			r = false;
			t = false;
			z = false;
			u = false;
			i = false;
			o = false;
			p = false;
			a = false;
			s = false;
			d = false;
			f = false;
			g = false;
			h = false;
			j = false;
			k = false;
			l = false;
			y = false;
			x = false;
			c = false;
			v = false;
			b = false;
			n = false;
			m = false;

			// Numbers
			key0 = false;
			key1 = false;
			key2 = false;
			key3 = false;
			key4 = false;
			key5 = false;
			key6 = false;
			key7 = false;
			key8 = false;
			key9 = false;

			// Signs
			period = false;
			comma = false;
			plus = false;
			minus = false;

			// Special
			esc = false;
			tab = false;
			caps = false;
			shift = false;
			ctrl = false;
			alt = false;
			space = false;
			enter = false;
			up = false;
			left = false;
			down = false;
			right = false;

			// Unknown
			unknown = 0;
		}

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
