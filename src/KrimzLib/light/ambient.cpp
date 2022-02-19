#include "KrimzLib/light/ambient.h"


// Returns the true light color
kl::vec4 kl::ambient::getCol() const {
	return color * intensity;
}
