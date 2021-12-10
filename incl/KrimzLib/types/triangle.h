#pragma once


namespace kl {
    struct triangle {
		kl::vertex vertices[3];

		// Constructors
		triangle() {
			vertices[0] = {};
			vertices[1] = {};
			vertices[2] = {};
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
		void computeInterConsts() {
			double tempConst = 1 / ((vertices[1].world.y - vertices[2].world.y) * (vertices[0].world.x - vertices[2].world.x) + (vertices[2].world.x - vertices[1].world.x) * (vertices[0].world.y - vertices[2].world.y));
			w1Const = kl::vec2((vertices[1].world.y - vertices[2].world.y) * tempConst, (vertices[2].world.x - vertices[1].world.x) * tempConst);
			w2Const = kl::vec2((vertices[2].world.y - vertices[0].world.y) * tempConst, (vertices[0].world.x - vertices[2].world.x) * tempConst);
		}

		// Calculates and returns the 3 barycentric weights of a triangle and a point
		kl::vec3 getInterWeights(kl::vec2 p) {
			double dx = p.x - vertices[2].world.x;
			double dy = p.y - vertices[2].world.y;
			double interWeight1 = dx * w1Const.x + dy * w1Const.y;
			double interWeight2 = dx * w2Const.x + dy * w2Const.y;
			return kl::vec3(interWeight1, interWeight2, 1 - interWeight1 - interWeight2);
		}

		// Checks if the point is inside the triangle
		bool inTriangle(kl::vec3 weights) {
			return !(weights.x < 0 || weights.y < 0 || weights.z < 0);
		}

		// Interpolates and returns a color of the barycentric weights
		kl::color interpolateColor(kl::vec3 weights) {
			return kl::color(
				vertices[0].color.r * weights.x + vertices[1].color.r * weights.y + vertices[2].color.r * weights.z,
				vertices[0].color.g * weights.x + vertices[1].color.g * weights.y + vertices[2].color.g * weights.z,
				vertices[0].color.b * weights.x + vertices[1].color.b * weights.y + vertices[2].color.b * weights.z
			);
		}

	private:
		kl::vec2 w1Const;
		kl::vec2 w2Const;
	};
}
