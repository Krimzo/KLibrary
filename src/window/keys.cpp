#include "KrimzLib/window/keys.h"

#include <windows.h>


void kl::keys::setKey(kl::key key, bool state) {
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
		num0 = state;
		break;
	case '1':
		num1 = state;
		break;
	case '2':
		num2 = state;
		break;
	case '3':
		num3 = state;
		break;
	case '4':
		num4 = state;
		break;
	case '5':
		num5 = state;
		break;
	case '6':
		num6 = state;
		break;
	case '7':
		num7 = state;
		break;
	case '8':
		num8 = state;
		break;
	case '9':
		num9 = state;
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
	case VK_INSERT:
		insert = state;
		break;
	case VK_DELETE:
		delet = state;
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

		// Function
	case VK_F1:
		f1 = state;
		break;
	case VK_F2:
		f2 = state;
		break;
	case VK_F3:
		f3 = state;
		break;
	case VK_F4:
		f4 = state;
		break;
	case VK_F5:
		f5 = state;
		break;
	case VK_F6:
		f6 = state;
		break;
	case VK_F7:
		f7 = state;
		break;
	case VK_F8:
		f8 = state;
		break;
	case VK_F9:
		f9 = state;
		break;
	case VK_F10:
		f10 = state;
		break;
	case VK_F11:
		f11 = state;
		break;
	case VK_F12:
		f12 = state;
		break;

		// Unknow
	default:
		unknown = key;
		break;
	}
}
