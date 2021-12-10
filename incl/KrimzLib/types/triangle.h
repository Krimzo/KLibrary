#pragma once


namespace kl {
    struct triangle {
	public:
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
		void computeInterConst() {
			double tempConst = 1 / ((vertices[1].y - vertices[2].y) * (vertices[0].x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[2].y));
			w1Const = kl::vec2((vertices[1].y - vertices[2].y) * tempConst, (vertices[2].x - vertices[1].x) * tempConst);
			w2Const = kl::vec2((vertices[2].y - vertices[0].y) * tempConst, (vertices[0].x - vertices[2].x) * tempConst);
		}

		// Calculates and returns the 3 barycentric weights of a triangle and a point
		kl::vec3 getInterWeights(kl::vec2 p) {
			double dx = p.x - vertices[2].x;
			double dy = p.y - vertices[2].y;
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
				vertices[0].c.r * weights.x + vertices[1].c.r * weights.y + vertices[2].c.r * weights.z,
				vertices[0].c.g * weights.x + vertices[1].c.g * weights.y + vertices[2].c.g * weights.z,
				vertices[0].c.b * weights.x + vertices[1].c.b * weights.y + vertices[2].c.b * weights.z
			);
		}

	private:
		kl::vec2 w1Const;
		kl::vec2 w2Const;
	};
}
