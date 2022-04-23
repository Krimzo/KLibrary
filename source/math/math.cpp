#include "math/math.h"

#include "utility/convert.h"


// Returns x from the given y of the line that goes through points a and b
float kl::math::lineX(const kl::float2& a, const kl::float2& b, float y) {
	return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
}

// Returns y from the given x of the line that goes through points a and b
float kl::math::lineY(const kl::float2& a, const kl::float2& b, float x) {
	return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
}

// Euluer/quat angles
kl::float4 kl::math::eulToQuat(const kl::float3& eul) {
	const float cr = cos(kl::convert::toRadians(eul.x) * 0.5f);
	const float sr = sin(kl::convert::toRadians(eul.x) * 0.5f);
	const float cp = cos(kl::convert::toRadians(eul.y) * 0.5f);
	const float sp = sin(kl::convert::toRadians(eul.y) * 0.5f);
	const float cy = cos(kl::convert::toRadians(eul.z) * 0.5f);
	const float sy = sin(kl::convert::toRadians(eul.z) * 0.5f);

	kl::float4 quat;
	quat.x = sr * cp * cy - cr * sp * sy;
	quat.y = cr * sp * cy + sr * cp * sy;
	quat.z = cr * cp * sy - sr * sp * cy;
	quat.w = cr * cp * cy + sr * sp * sy;
	return quat;
}
kl::float3 kl::math::quatToEul(const kl::float4& quat) {
	const float sinp = 2.0f * (quat.w * quat.y - quat.z * quat.x);
	const float sinrCosp = 2.0f * (quat.w * quat.x + quat.y * quat.z);
	const float cosrCosp = 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y);
	const float sinyCosp = 2.0f * (quat.w * quat.z + quat.x * quat.y);
	const float cosyCosp = 1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z);

	kl::float3 eul;
	eul.x = kl::convert::toDegrees(std::atan2(sinrCosp, cosrCosp));
	eul.y = kl::convert::toDegrees((std::abs(sinp) >= 1.0f) ? std::copysign(kl::math::pi * 0.5f, sinp) : std::asin(sinp));
	eul.z = kl::convert::toDegrees(std::atan2(sinyCosp, cosyCosp));
	return eul;
}
