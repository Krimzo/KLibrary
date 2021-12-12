#pragma once


namespace kl {
    struct triangle {
		kl::vertex vertices[3];

		// Constructors
		triangle() {
			vertices[0] = kl::vertex();
			vertices[1] = kl::vertex();
			vertices[2] = kl::vertex();
		}
		triangle(kl::vertex v0, kl::vertex v1, kl::vertex v2) {
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
		}

		// Returns raw data pointer
		void* pointer() {
			return (void*)vertices;
		}

		// Computes and stores the barycentric constant
		kl::vec4 computeInterConsts() {
			float tempConst = 1 / ((vertices[1].world.y - vertices[2].world.y) * (vertices[0].world.x - vertices[2].world.x) + (vertices[2].world.x - vertices[1].world.x) * (vertices[0].world.y - vertices[2].world.y));
			return vec4(
				(vertices[1].world.y - vertices[2].world.y) * tempConst,
				(vertices[2].world.x - vertices[1].world.x) * tempConst,
				(vertices[2].world.y - vertices[0].world.y) * tempConst,
				(vertices[0].world.x - vertices[2].world.x) * tempConst
			);
		}

		// Calculates and returns the 3 barycentric weights of a triangle and a point
		kl::vec3 getInterWeights(kl::vec4 interConsts, kl::vec2 p) {
			float dx = p.x - vertices[2].world.x;
			float dy = p.y - vertices[2].world.y;
			float interWeight1 = dx * interConsts.x + dy * interConsts.y;
			float interWeight2 = dx * interConsts.z + dy * interConsts.w;
			return kl::vec3(interWeight1, interWeight2, 1 - interWeight1 - interWeight2);
		}

		// Checks if the point is inside the triangle
		bool inTriangle(kl::vec3 weights) {
			return !(weights.x < 0 || weights.y < 0 || weights.z < 0);
		}

		// Interpolates and returns the depth
		float interpolateDepth(kl::vec3 weights) {
			return vertices[0].world.z * weights.x + vertices[1].world.z * weights.y + vertices[2].world.z * weights.z;
		}

		// Interpolates and returns the uv
		kl::vec2 interpolateUV(kl::vec3 weights) {
			return kl::vec2(
				vertices[0].texture.x * weights.x + vertices[1].texture.x * weights.y + vertices[2].texture.x * weights.z,
				vertices[0].texture.y * weights.x + vertices[1].texture.y * weights.y + vertices[2].texture.y * weights.z
			);
		}

		// Interpolates and returns the color
		kl::colorf interpolateColor(kl::vec3 weights) {
			return kl::colorf(
				vertices[0].color.r * weights.x + vertices[1].color.r * weights.y + vertices[2].color.r * weights.z,
				vertices[0].color.g * weights.x + vertices[1].color.g * weights.y + vertices[2].color.g * weights.z,
				vertices[0].color.b * weights.x + vertices[1].color.b * weights.y + vertices[2].color.b * weights.z
			);
		}
	};
}
