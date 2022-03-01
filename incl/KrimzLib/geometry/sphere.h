#pragma once

#include "KrimzLib/vecmath/vec3.h"


namespace kl {
	class sphere {
	public:
        // Geometry
        kl::vec3 center;
        float radius = 0;
        
        // Light
        kl::vec3 color;
        float reflectivity = 0;
        float emission = 0;

        // Constructor
        sphere();
        sphere(const kl::vec3& center, float radius, const kl::vec3& color, float reflectivity = 0, float emission = 0);

        // Calculates the emissive color
        kl::vec3 calcEmiss() const;
	};
}
