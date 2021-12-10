#pragma once 


namespace kl {
    struct vertex {
		double x;
		double y;
		double z;
		double u;
		double v;
		kl::color c;

		// Constructors
		vertex() {
			x = 0;
			y = 0;
			z = 0;
			u = 0;
			v = 0;
			c = {};
		}
		vertex(double x, double y) {
			this->x = x;
			this->y = y;
			z = 0;
			u = 0;
			v = 0;
			c = {};
		}
		vertex(double x, double y, double z) {
			this->x = x;
			this->y = y;
			this->z = z;
			u = 0;
			v = 0;
			c = {};
		}
		vertex(double x, double y, double z, double u, double v) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->u = u;
			this->v = v;
			c = {};
		}
	};
}