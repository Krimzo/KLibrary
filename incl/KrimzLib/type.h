#pragma once


/* PRIMITIVES */
namespace kl {
	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef long long int64;

	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;
}

/* MATH */
namespace kl {
	struct size {
		uint32 width;
		uint32 height;

		// Constructor
		size(uint32 width = 0, uint32 height = 0) {
			this->width = width;
			this->height = height;
		}

		// Operator overloading
		bool operator == (size obj) {
			return (width == obj.width && height == obj.height);
		}
		bool operator != (size obj) {
			return (width != obj.width || height != obj.height);
		}
	};

	struct point {
		int x;
		int y;

		// Constructor
		point(int x = 0, int y = 0) {
			this->x = x;
			this->y = y;
		}

		// Operator overloading
		bool operator == (point obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (point obj) {
			return (x != obj.x || y != obj.y);
		}
	};

	struct vec2 {
		double x;
		double y;

		// Constructors
		vec2(double x = 0, double y = 0) {
			this->x = x;
			this->y = y;
		}
		vec2(point p) {
			x = (double)p.x;
			y = (double)p.y;
		}
		vec2(point a, point b) {
			x = double(b.x - a.x);
			y = double(b.y - a.y);
		}

		// Operator overloading
		vec2 operator + (vec2 obj) {
			return { x + obj.x, y + obj.y };
		}
		vec2 operator - (vec2 obj) {
			return { x - obj.x, y - obj.y };
		}
		vec2 operator * (double a) {
			return { x * a, y * a };
		}
		vec2 operator / (double a) {
			return { x / a, y / a };
		}
		bool operator == (vec2 obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (vec2 obj) {
			return (x != obj.x || y != obj.y);
		}

		// Returns the vectors lenght
		double Lenght() {
			return sqrt(x * x + y * y);
		}

		// Normalizes a vector
		void Normalize() {
			double vecLen = this->Lenght();
			x /= vecLen;
			y /= vecLen;
		}

		// Retruns a normalized vector
		vec2 Normalized() {
			double vecLen = this->Lenght();
			return { x / vecLen, y / vecLen };
		}
	};

	struct vec3 {
		double x;
		double y;
		double z;

		// Constructor
		vec3(double x = 0, double y = 0, double z = 0) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		// Operator overloading
		vec3 operator + (vec3 obj) {
			return { x + obj.x, y + obj.y, z + obj.z };
		}
		vec3 operator - (vec3 obj) {
			return { x - obj.x, y - obj.y, z - obj.z };
		}
		vec3 operator * (double a) {
			return { x * a, y * a, z * a };
		}
		vec3 operator / (double a) {
			return { x / a, y / a, z / a };
		}
		bool operator == (vec3 obj) {
			return (x == obj.x && y == obj.y && z == obj.z);
		}
		bool operator != (vec3 obj) {
			return (x != obj.x || y != obj.y || z != obj.z);
		}

		// Returns the vectors lenght
		double Lenght() {
			return sqrt(x * x + y * y + z * z);
		}

		// Normalizes a vector
		void Normalize() {
			double vecLen = this->Lenght();
			x /= vecLen;
			y /= vecLen;
			z /= vecLen;
		}

		// Retruns a normalized vector
		vec3 Normalized() {
			double vecLen = this->Lenght();
			return { x / vecLen, y / vecLen, z / vecLen };
		}
	};
}

/* BINARY */
namespace kl {
	typedef unsigned char byte;
	typedef std::vector<byte> bytes;
	typedef std::string bits;
}

/* IMAGE */
namespace kl {
	struct colord {
		double r;
		double g;
		double b;

		// Constructor
		colord(double r = 0, double g = 0, double b = 0) {
			this->r = r;
			this->g = g;
			this->b = b;
		}

		// Operator overloading
		bool operator == (colord obj) {
			return (r == obj.r && g == obj.g && b == obj.b);
		}
		bool operator != (colord obj) {
			return (r != obj.r || g != obj.g || b != obj.b);
		}
	};

	struct color {
		// Had to flip R and B because the winapi uses BGR
		byte b;
		byte g;
		byte r;

		color(byte r = 0, byte g = 0, byte b = 0) {
			this->r = r;
			this->g = g;
			this->b = b;
		}

		// Conversions
		operator colord() const {
			return { r / 255.0, g / 255.0, b / 255.0 };
		}

		// Operator overloading
		bool operator == (color obj) {
			return (r == obj.r && g == obj.g && b == obj.b);
		}
		bool operator != (color obj) {
			return (r != obj.r || g != obj.g || b != obj.b);
		}
	};
}

/* OPENGL */
namespace kl {
	typedef unsigned int texture;
}

/* WINDOW */
namespace kl {
	typedef unsigned int key;

	struct mouse {
		bool lmb = false;
		bool mmb = false;
		bool rmb = false;
		point position = {};
	};
}

/* ENGINE */
namespace kl {
	struct vertex {
		double x = 0;
		double y = 0;
		double z = 0;
		double u = 0;
		double v = 0;
		colord color = {};
	};

	struct triangle {
		vertex vertices[3] = {};
		bool textured = false;
	};

	struct gameobject {
		// Visual
		bool visible = true;
		texture texture = 0;
		std::vector<triangle> triangles = {};

		// Geometry
		vec3 position = {};
		vec3 rotation = {};
		vec3 size = { 1, 1, 1 };

		// Physics
		bool physics = false;
		double gravity = 1;
		vec3 velocity = {};
		vec3 angular = {};
	};

	struct camera {
		vec3 position = {};
		vec3 rotation = {};
	};
}
