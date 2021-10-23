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
	struct point {
		int x = 0;
		int y = 0;
	};

	struct size {
		int width = 0;
		int height = 0;
	};

	struct vec2 {
		double x = 0;
		double y = 0;

		// Operator overloading
		vec2 operator + (vec2 obj) {
			return { x + obj.x, y + obj.y };
		}
		vec2 operator - (vec2 obj) {
			return { x - obj.x, y - obj.y };
		}
		vec2 operator * (int obj) {
			return { x * obj, y * obj };
		}
		vec2 operator * (double obj) {
			return { x * obj, y * obj };
		}
		vec2 operator / (int obj) {
			return { x / obj, y / obj };
		}
		vec2 operator / (double obj) {
			return { x / obj, y / obj };
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
		double x = 0;
		double y = 0;
		double z = 0;

		// Operator overloading
		vec3 operator + (vec3 obj) {
			return { x + obj.x, y + obj.y, z + obj.z };
		}
		vec3 operator - (vec3 obj) {
			return { x - obj.x, y - obj.y, z - obj.z };
		}
		vec3 operator * (int obj) {
			return { x * obj, y * obj, z * obj };
		}
		vec3 operator * (double obj) {
			return { x * obj, y * obj, z * obj };
		}
		vec3 operator / (int obj) {
			return { x / obj, y / obj, z / obj };
		}
		vec3 operator / (double obj) {
			return { x / obj, y / obj, z / obj };
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

/* IMAGE */
namespace kl {
	typedef unsigned char byte;

	struct colorf {
		float r = 0;
		float g = 0;
		float b = 0;
	};

	struct colord {
		double r = 0;
		double g = 0;
		double b = 0;
	};

	struct color {
		byte r = 0;
		byte g = 0;
		byte b = 0;

		// Conversions
		operator colorf() const {
			return { r / 255.0f, g / 255.0f, b / 255.0f };
		}
		operator colord() const {
			return { r / 255.0, g / 255.0, b / 255.0 };
		}
	};
}

/* BINARY */
namespace kl {
	typedef std::vector<byte> bytes;
	typedef std::string bits;
}

/* OPENCL */
namespace kl {
	typedef void* cpumem;
	typedef cl_mem gpumem;
	typedef cl_program clprogram;
	typedef cl_kernel clkernel;
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
		double gravityMulti = 1;
		vec3 velocity = {};
		vec3 angularMo = {};
	};

	struct camera {
		vec3 position = {};
		vec3 rotation = {};
	};
}
