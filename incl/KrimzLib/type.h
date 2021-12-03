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
		kl::uint32 width;
		kl::uint32 height;

		// Constructors
		size() {
			width = 0;
			height = 0;
		}
		size(kl::uint32 width, kl::uint32 height) {
			this->width = width;
			this->height = height;
		}

		// Operator overloading
		bool operator == (kl::size obj) {
			return (width == obj.width && height == obj.height);
		}
		bool operator != (kl::size obj) {
			return (width != obj.width || height != obj.height);
		}
	};

	struct point {
		int x;
		int y;

		// Constructors
		point() {
			x = 0;
			y = 0;
		}
		point(int x, int y) {
			this->x = x;
			this->y = y;
		}

		// Operator overloading
		bool operator == (kl::point obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (kl::point obj) {
			return (x != obj.x || y != obj.y);
		}
	};

	struct vec2 {
		double x;
		double y;

		// Constructors
		vec2() {
			x = 0;
			y = 0;
		}
		vec2(double x, double y) {
			this->x = x;
			this->y = y;
		}
		vec2(kl::point p) {
			x = double(p.x);
			y = double(p.y);
		}
		vec2(kl::point a, kl::point b) {
			x = double(b.x - a.x);
			y = double(b.y - a.y);
		}

		// Operator overloading
		kl::vec2 operator + (kl::vec2 obj) {
			return kl::vec2(x + obj.x, y + obj.y);
		}
		kl::vec2 operator - (kl::vec2 obj) {
			return kl::vec2(x - obj.x, y - obj.y);
		}
		kl::vec2 operator * (double a) {
			return kl::vec2(x * a, y * a);
		}
		kl::vec2 operator / (double a) {
			return kl::vec2(x / a, y / a);
		}
		bool operator == (kl::vec2 obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (kl::vec2 obj) {
			return (x != obj.x || y != obj.y);
		}

		// Returns the vectors length
		double Length() {
			return sqrt(x * x + y * y);
		}

		// Normalizes a vector
		void Normalize() {
			double vecLen = this->Length();
			x /= vecLen;
			y /= vecLen;
		}

		// Retruns a normalized vector
		kl::vec2 Normalized() {
			double vecLen = this->Length();
			return kl::vec2(x / vecLen, y / vecLen);
		}
	};

	struct vec3 {
		double x;
		double y;
		double z;

		// Constructors
		vec3() {
			x = 0;
			y = 0;
			z = 0;
		}
		vec3(double x, double y, double z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		// Operator overloading
		kl::vec3 operator + (kl::vec3 obj) {
			return kl::vec3(x + obj.x, y + obj.y, z + obj.z);
		}
		kl::vec3 operator - (kl::vec3 obj) {
			return kl::vec3(x - obj.x, y - obj.y, z - obj.z);
		}
		kl::vec3 operator * (double a) {
			return kl::vec3(x * a, y * a, z * a);
		}
		kl::vec3 operator / (double a) {
			return kl::vec3(x / a, y / a, z / a);
		}
		bool operator == (kl::vec3 obj) {
			return (x == obj.x && y == obj.y && z == obj.z);
		}
		bool operator != (kl::vec3 obj) {
			return (x != obj.x || y != obj.y || z != obj.z);
		}

		// Returns the vectors length
		double Length() {
			return sqrt(x * x + y * y + z * z);
		}

		// Normalizes a vector
		void Normalize() {
			double vecLen = this->Length();
			x /= vecLen;
			y /= vecLen;
			z /= vecLen;
		}

		// Retruns a normalized vector
		kl::vec3 Normalized() {
			double vecLen = this->Length();
			return kl::vec3(x / vecLen, y / vecLen, z / vecLen);
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

		// Constructors
		colord() {
			r = 0;
			g = 0;
			b = 0;
		}
		colord(double r, double g, double b) {
			this->r = r;
			this->g = g;
			this->b = b;
		}

		// Operator overloading
		bool operator == (kl::colord obj) {
			return (r == obj.r && g == obj.g && b == obj.b);
		}
		bool operator != (kl::colord obj) {
			return (r != obj.r || g != obj.g || b != obj.b);
		}
	};

	struct color {
		// Had to flip R and B because the winapi uses BGR
		kl::byte b;
		kl::byte g;
		kl::byte r;

		// Constructors
		color() {
			r = 0;
			g = 0;
			b = 0;
		}
		color(kl::byte r, kl::byte g, kl::byte b) {
			this->r = r;
			this->g = g;
			this->b = b;
		}

		// Conversions
		operator kl::colord() const {
			return kl::colord(r / 255.0, g / 255.0, b / 255.0);
		}

		// Operator overloading
		bool operator == (kl::color obj) {
			return (r == obj.r && g == obj.g && b == obj.b);
		}
		bool operator != (kl::color obj) {
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
		bool lmb;
		bool mmb;
		bool rmb;
		kl::point position;

		// Constructors
		mouse() {
			lmb = false;
			mmb = false;
			rmb = false;
			position = kl::point();
		}
		mouse(bool lmb, bool mmb, bool rmb, kl::point position) {
			this->lmb = lmb;
			this->mmb = mmb;
			this->rmb = rmb;
			this->position = position;
		}
	};
}

/* ENGINE */
namespace kl {
	typedef unsigned int id;

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

	struct triangle {
	public:
		kl::vertex vertices[3];

		// Constructors
		triangle() {
			for (int i = 0; i < 3; i++) {
				vertices[i] = kl::vertex();
			}
			w1Const = {};
			w2Const = {};
		}
		triangle(kl::vertex v0, kl::vertex v1, kl::vertex v2) {
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
			w1Const = {};
			w2Const = {};
		}

		// Computes and stores the barycentric constant
		void computeBaryConst() {
			double tempConst = 1 / ((vertices[1].y - vertices[2].y) * (vertices[0].x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[2].y));
			w1Const = kl::vec2((vertices[1].y - vertices[2].y) * tempConst, (vertices[2].x - vertices[1].x) * tempConst);
			w2Const = kl::vec2((vertices[2].y - vertices[0].y) * tempConst, (vertices[0].x - vertices[2].x) * tempConst);
		}

		// Calculates and returns the 3 barycentric weights of a triangle and a point
		kl::vec3 getBaryWeights(kl::vec2 p) {
			double dx = p.x - vertices[2].x;
			double dy = p.y - vertices[2].y;
			double baryWeight1 = dx * w1Const.x + dy * w1Const.y;
			double baryWeight2 = dx * w2Const.x + dy * w2Const.y;
			return kl::vec3(baryWeight1, baryWeight2, 1 - baryWeight1 - baryWeight2);
		}

		// Checks if the point is inside the triangle
		bool inTriangle(kl::vec3 weights) {
			return !(weights.x < 0 || weights.y < 0 || weights.z < 0);
		}

		// Interpolates and returns a color of the barycentric weights
		kl::color interpolateColor(kl::vec3 weights) {
			return kl::color(
				vertices[0].c.r * weights.x + vertices[1].c.r * weights.y + vertices[2].c.r * weights.z,
				vertices[0].c.g * weights.x + vertices[1].c.g * weights.y + vertices[2].c.g * weights.z,
				vertices[0].c.b * weights.x + vertices[1].c.b * weights.y + vertices[2].c.b * weights.z
			);
		}

	private:
		kl::vec2 w1Const;
		kl::vec2 w2Const;
	};

	struct gameobject {
		// Visual
		bool visible;
		kl::texture texture;
		std::vector<kl::triangle> triangles;

		// Geometry
		kl::vec3 position;
		kl::vec3 rotation;
		kl::vec3 size;

		// Physics
		bool physics;
		double gravity;
		kl::vec3 velocity;
		kl::vec3 angular;

		// Constructors
		gameobject() {
			visible = true;
			texture = 0;
			triangles = {};
			position = kl::vec3();
			rotation = kl::vec3();
			size = kl::vec3(1, 1, 1);
			physics = false;
			gravity = 1;
			velocity = kl::vec3();
			angular = kl::vec3();
		}
		gameobject(kl::texture textureID) {
			visible = true;
			texture = textureID;
			triangles = {};
			position = kl::vec3();
			rotation = kl::vec3();
			size = kl::vec3(1, 1, 1);
			physics = false;
			gravity = 1;
			velocity = kl::vec3();
			angular = kl::vec3();
		}
	};

	struct camera {
		kl::vec3 position;
		kl::vec3 rotation;

		// Constructors
		camera() {
			position = kl::vec3();
			rotation = kl::vec3();
		}
		camera(kl::vec3 position, kl::vec3 rotation) {
			this->position = position;
			this->rotation = rotation;
		}
	};
}
