#pragma once

#include "KrimzLib/vecmath/vec3.h"


namespace kl {
	class sphere {
	public:
        // Geometry
        kl::vec3 center;
        float radius = 0;
        
        // Colors
        kl::vec3 surfColor;
        kl::vec3 emissColor;

        // View
        float transparency = 0;
        float reflection = 0;

        // Constructor
        sphere();
        sphere(const kl::vec3& center, float radius, const kl::vec3& surfColor, const kl::vec3& emissColor, float transparency, float reflection);
	};
}
