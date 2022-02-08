#include "KrimzLib/renderer3D/light/ambient.h"


// Returns the true light color
kl::vec3 kl::ambient::getCol() {
	return color * intensity;
}
