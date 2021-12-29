#pragma once


namespace kl {
	struct triangle {
		kl::vec2 a;
		kl::vec2 b;
		kl::vec2 c;

		// Constructors
		triangle() {}
		triangle(kl::vec2 a, kl::vec2 b, kl::vec2 c) {
			this->a = a;
			this->b = b;
			this->c = c;
		}

		// Computes and stores the barycentric constants
		void calcConsts() {
			const float tempConst = 1 / ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
			interConsts = kl::vec4(
				(b.y - c.y) * tempConst,
				(c.x - b.x) * tempConst,
				(c.y - a.y) * tempConst,
				(a.x - c.x) * tempConst
			);
		}

		// Checks if the point is inside the triangle
		bool inTriangle(kl::vec2 pos) {
			kl::vec3 weights = getWeights(pos);
			return !(weights.x < 0 || weights.y < 0 || weights.z < 0);
		}

		// Interpolates and returns the depth
		float interpolate(kl::vec3 values, kl::vec2 pos) {
			kl::vec3 weights = getWeights(pos);
			return values.x * weights.x + values.y * weights.y + values.z * weights.z;
		}

	private:
		// Interpolation constants
		kl::vec4 interConsts;

		// Calculates and returns the 3 barycentric weights of a triangle and a point
		kl::vec3 getWeights(kl::vec2 pos) {
			const float dx = pos.x - c.x;
			const float dy = pos.y - c.y;
			const float interWeight1 = dx * interConsts.x + dy * interConsts.y;
			const float interWeight2 = dx * interConsts.z + dy * interConsts.w;
			return kl::vec3(interWeight1, interWeight2, 1 - interWeight1 - interWeight2);
		}
	};
}