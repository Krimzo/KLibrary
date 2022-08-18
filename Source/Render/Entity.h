#pragma once

#include "KLTypes.h"
#include "GPU/DXTypes.h"
#include "Math/Math.h"


namespace kl {
	class Entity {
	public:
		Float3 scale = { 1.0f, 1.0f, 1.0f };
		Float3 position = {};
		Float3 rotation = {};

		Float3 acceleration = {};
		Float3 velocity = {};
		Float3 angular = {};

		dx::Buffer mesh = nullptr;
		dx::ShaderView texture = nullptr;
		Float4 color = Colors::Gray;

		Entity();

		void updatePhysics(float deltaTime);

		Mat4 matrix() const;
	};
}
