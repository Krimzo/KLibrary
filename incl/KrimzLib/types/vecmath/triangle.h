#pragma once


namespace kl {
	struct triangle {
		kl::vertex vertices[3] = {};

		// Returns raw data pointer
		void* pointer() {
			return (void*)vertices;
		}

		// Computes and stores the barycentric constant
		kl::vec4 getInterConsts() {
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
		float getDepth(kl::vec3 weights) {
			return vertices[0].world.z * weights.x + vertices[1].world.z * weights.y + vertices[2].world.z * weights.z;
		}

		// Interpolates and returns the uv
		kl::vec2 getUV(kl::vec3 weights) {
			return kl::vec2(
				vertices[0].texture.x * weights.x + vertices[1].texture.x * weights.y + vertices[2].texture.x * weights.z,
				vertices[0].texture.y * weights.x + vertices[1].texture.y * weights.y + vertices[2].texture.y * weights.z
			);
		}
	};
}