#include "window/keys.h"

#include <windows.h>


void kl::keys::update(int64 key, bool state) {
	switch (key) {
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

void kl::keys::update() const {
	q.update();
	w.update();
	e.update();
	r.update();
	t.update();
	z.update();
	u.update();
	i.update();
	o.update();
	p.update();
	a.update();
	s.update();
	d.update();
	f.update();
	g.update();
	h.update();
	j.update();
	k.update();
	l.update();
	y.update();
	x.update();
	c.update();
	v.update();
	b.update();
	n.update();
	m.update();
	num0.update();
	num1.update();
	num2.update();
	num3.update();
	num4.update();
	num5.update();
	num6.update();
	num7.update();
	num8.update();
	num9.update();
	period.update();
	comma.update();
	plus.update();
	minus.update();
	esc.update();
	tab.update();
	caps.update();
	shift.update();
	ctrl.update();
	alt.update();
	space.update();
	enter.update();
	insert.update();
	delet.update();
	up.update();
	left.update();
	down.update();
	right.update();
	f1.update();
	f2.update();
	f3.update();
	f4.update();
	f5.update();
	f6.update();
	f7.update();
	f8.update();
	f9.update();
	f10.update();
	f11.update();
	f12.update();
}
