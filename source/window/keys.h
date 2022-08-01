#pragma once

#include "window/key.h"
#include "math/math.h"


namespace kl {
	struct keys {
		union {
			struct {
				kl::key q;
				kl::key w;
				kl::key e;
				kl::key r;
				kl::key t;
				kl::key z;
				kl::key u;
				kl::key i;
				kl::key o;
				kl::key p;
				kl::key a;
				kl::key s;
				kl::key d;
				kl::key f;
				kl::key g;
				kl::key h;
				kl::key j;
				kl::key k;
				kl::key l;
				kl::key y;
				kl::key x;
				kl::key c;
				kl::key v;
				kl::key b;
				kl::key n;
				kl::key m;

				kl::key num0;
				kl::key num1;
				kl::key num2;
				kl::key num3;
				kl::key num4;
				kl::key num5;
				kl::key num6;
				kl::key num7;
				kl::key num8;
				kl::key num9;

				kl::key period;
				kl::key comma;
				kl::key plus;
				kl::key minus;

				kl::key esc;
				kl::key tab;
				kl::key caps;
				kl::key shift;
				kl::key ctrl;
				kl::key alt;
				kl::key space;
				kl::key enter;
				kl::key insert;
				kl::key delet;
				kl::key up;
				kl::key left;
				kl::key down;
				kl::key right;

				kl::key f1;
				kl::key f2;
				kl::key f3;
				kl::key f4;
				kl::key f5;
				kl::key f6;
				kl::key f7;
				kl::key f8;
				kl::key f9;
				kl::key f10;
				kl::key f11;
				kl::key f12;
			};
			kl::key buffer[66];
		};

		keys();
		~keys();

		void update(int64 type, bool state);
		void update() const;
	};

	using keyboard = kl::keys;
}
