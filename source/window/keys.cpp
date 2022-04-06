#include "window/keys.h"

#include <windows.h>


void kl::keys::updateKey(kl::id key, bool state)
{
	switch (key)
	{
		// Letters
	case 'Q':
		q.update(state);
		break;
	case 'W':
		w.update(state);
		break;
	case 'E':
		e.update(state);
		break;
	case 'R':
		r.update(state);
		break;
	case 'T':
		t.update(state);
		break;
	case 'Z':
		z.update(state);
		break;
	case 'U':
		u.update(state);
		break;
	case 'I':
		i.update(state);
		break;
	case 'O':
		o.update(state);
		break;
	case 'P':
		p.update(state);
		break;
	case 'A':
		a.update(state);
		break;
	case 'S':
		s.update(state);
		break;
	case 'D':
		d.update(state);
		break;
	case 'F':
		f.update(state);
		break;
	case 'G':
		g.update(state);
		break;
	case 'H':
		h.update(state);
		break;
	case 'J':
		j.update(state);
		break;
	case 'K':
		k.update(state);
		break;
	case 'L':
		l.update(state);
		break;
	case 'Y':
		y.update(state);
		break;
	case 'X':
		x.update(state);
		break;
	case 'C':
		c.update(state);
		break;
	case 'V':
		v.update(state);
		break;
	case 'B':
		b.update(state);
		break;
	case 'N':
		n.update(state);
		break;
	case 'M':
		m.update(state);
		break;

		// Numbers
	case '0':
		num0.update(state);
		break;
	case '1':
		num1.update(state);
		break;
	case '2':
		num2.update(state);
		break;
	case '3':
		num3.update(state);
		break;
	case '4':
		num4.update(state);
		break;
	case '5':
		num5.update(state);
		break;
	case '6':
		num6.update(state);
		break;
	case '7':
		num7.update(state);
		break;
	case '8':
		num8.update(state);
		break;
	case '9':
		num9.update(state);
		break;

		// Signs
	case VK_OEM_PERIOD:
		period.update(state);
		break;
	case VK_OEM_COMMA:
		comma.update(state);
		break;
	case VK_OEM_PLUS:
		plus.update(state);
		break;
	case VK_OEM_MINUS:
		minus.update(state);
		break;

		// Special
	case VK_ESCAPE:
		esc.update(state);
		break;
	case VK_TAB:
		tab.update(state);
		break;
	case VK_CAPITAL:
		caps.update(state);
		break;
	case VK_SHIFT:
		shift.update(state);
		break;
	case VK_CONTROL:
		ctrl.update(state);
		break;
	case VK_MENU:
		alt.update(state);
		break;
	case VK_SPACE:
		space.update(state);
		break;
	case VK_RETURN:
		enter.update(state);
		break;
	case VK_INSERT:
		insert.update(state);
		break;
	case VK_DELETE:
		delet.update(state);
		break;
	case VK_UP:
		up.update(state);
		break;
	case VK_LEFT:
		left.update(state);
		break;
	case VK_DOWN:
		down.update(state);
		break;
	case VK_RIGHT:
		right.update(state);
		break;

		// Function
	case VK_F1:
		f1.update(state);
		break;
	case VK_F2:
		f2.update(state);
		break;
	case VK_F3:
		f3.update(state);
		break;
	case VK_F4:
		f4.update(state);
		break;
	case VK_F5:
		f5.update(state);
		break;
	case VK_F6:
		f6.update(state);
		break;
	case VK_F7:
		f7.update(state);
		break;
	case VK_F8:
		f8.update(state);
		break;
	case VK_F9:
		f9.update(state);
		break;
	case VK_F10:
		f10.update(state);
		break;
	case VK_F11:
		f11.update(state);
		break;
	case VK_F12:
		f12.update(state);
		break;

		// Unknown
	default:
		unknown = key;
		break;
	}
}

// Calls all key down functions
void kl::keys::callAllDowns()
{
	if (q) q.down();
	if (w) w.down();
	if (e) e.down();
	if (r) r.down();
	if (t) t.down();
	if (z) z.down();
	if (u) u.down();
	if (i) i.down();
	if (o) o.down();
	if (p) p.down();
	if (a) a.down();
	if (s) s.down();
	if (d) d.down();
	if (f) f.down();
	if (g) g.down();
	if (h) h.down();
	if (j) j.down();
	if (k) k.down();
	if (l) l.down();
	if (y) y.down();
	if (x) x.down();
	if (c) c.down();
	if (v) v.down();
	if (b) b.down();
	if (n) n.down();
	if (m) m.down();
	if (num0) num0.down();
	if (num1) num1.down();
	if (num2) num2.down();
	if (num3) num3.down();
	if (num4) num4.down();
	if (num5) num5.down();
	if (num6) num6.down();
	if (num7) num7.down();
	if (num8) num8.down();
	if (num9) num9.down();
	if (period) period.down();
	if (comma) comma.down();
	if (plus) plus.down();
	if (minus) minus.down();
	if (esc) esc.down();
	if (tab) tab.down();
	if (caps) caps.down();
	if (shift) shift.down();
	if (ctrl) ctrl.down();
	if (alt) alt.down();
	if (space) space.down();
	if (enter) enter.down();
	if (insert) insert.down();
	if (delet) delet.down();
	if (up) up.down();
	if (left) left.down();
	if (down) down.down();
	if (right) right.down();
	if (f1) f1.down();
	if (f2) f2.down();
	if (f3) f3.down();
	if (f4) f4.down();
	if (f5) f5.down();
	if (f6) f6.down();
	if (f7) f7.down();
	if (f8) f8.down();
	if (f9) f9.down();
	if (f10) f10.down();
	if (f11) f11.down();
	if (f12) f12.down();
}
