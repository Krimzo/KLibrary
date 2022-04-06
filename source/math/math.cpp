#include "math/math.h"


// Returns x from the given y of the line that goes through points a and b
float kl::math::lineX(const kl::float2& a, const kl::float2& b, float y)
{
	return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
}

// Returns y from the given x of the line that goes through points a and b
float kl::math::lineY(const kl::float2& a, const kl::float2& b, float x)
{
	return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
}
