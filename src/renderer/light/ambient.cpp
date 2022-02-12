#include "KrimzLib/renderer/light/ambient.h"


// Returns the true light color
kl::vec3 kl::ambient::getCol() const {
	return color * intensity;
}
