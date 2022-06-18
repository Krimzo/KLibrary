#pragma once

#include "math/vector2.cuh"
#include "math/vector3.cuh"
#include "math/vector4.cuh"
#include "math/matrix2x2.cuh"
#include "math/matrix3x3.cuh"
#include "math/matrix4x4.cuh"


namespace kl::math {
	// Returns x from the given y of the line that goes through points a and b
	template<typename T> ALL inline T lineX(const kl::vector2<T>& a, const kl::vector2<T>& b, const T& y) {
		return T(((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x);
	}

	// Returns y from the given x of the line that goes through points a and b
	template<typename T> ALL inline T lineY(const kl::vector2<T>& a, const kl::vector2<T>& b, const T& x) {
		return T(((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y);
	}

	// Euluer/quat angles
	template<typename T> ALL inline kl::vector4<T> toQuat(const kl::vector3<T>& eul) {
		const T cr = T(std::cos(kl::math::toRads(eul.x) * 0.5));
		const T sr = T(std::sin(kl::math::toRads(eul.x) * 0.5));
		const T cp = T(std::cos(kl::math::toRads(eul.y) * 0.5));
		const T sp = T(std::sin(kl::math::toRads(eul.y) * 0.5));
		const T cy = T(std::cos(kl::math::toRads(eul.z) * 0.5));
		const T sy = T(std::sin(kl::math::toRads(eul.z) * 0.5));
		return {
			sr * cp * cy - cr * sp * sy,
			cr * sp * cy + sr * cp * sy,
			cr * cp * sy - sr * sp * cy,
			cr * cp * cy + sr * sp * sy
		};
	}
	template<typename T> ALL inline kl::vector3<T> toEul(const kl::vector4<T>& quat) {
		const T sinp = T(2.0 * (quat.w * quat.y - quat.z * quat.x));
		const T sinrCosp = T(2.0 * (quat.w * quat.x + quat.y * quat.z));
		const T cosrCosp = T(1.0 - 2.0 * (quat.x * quat.x + quat.y * quat.y));
		const T sinyCosp = T(2.0 * (quat.w * quat.z + quat.x * quat.y));
		const T cosyCosp = T(1.0 - 2.0 * (quat.y * quat.y + quat.z * quat.z));
		return {
			kl::math::toDegs(std::atan2(sinrCosp, cosrCosp)),
			kl::math::toDegs((std::abs(sinp) >= 1.0) ? std::copysign(1.57079632679, sinp) : std::asin(sinp)),
			kl::math::toDegs(std::atan2(sinyCosp, cosyCosp))
		};
	}

	// minmax
	template<typename T> ALL inline T minmax(const T& val, const T& minVal, const T& maxVal) {
		return min(max(val, minVal), maxVal);
	}
}
