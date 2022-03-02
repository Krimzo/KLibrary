#include "KrimzLib/light/ambient.h"


// Returns the true light color
kl::float4 kl::ambient::getCol() const {
	return color * intensity;
}
