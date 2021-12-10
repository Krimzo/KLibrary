#pragma once


namespace kl {
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
}