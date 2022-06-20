#pragma once

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/matrix2x2.h"
#include "math/matrix3x3.h"
#include "math/matrix4x4.h"


namespace kl {
	namespace math {
		// Returns x from the given y of the line that goes through points a and b
		template<typename T> inline T lineX(const kl::vector2<T>& a, const kl::vector2<T>& b, const T& y) {
			return T(((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x);
		}

		// Returns y from the given x of the line that goes through points a and b
		template<typename T> inline T lineY(const kl::vector2<T>& a, const kl::vector2<T>& b, const T& x) {
			return T(((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y);
		}

		// Euluer/quat angles
		template<typename T> inline kl::vector4<T> toQuat(const kl::vector3<T>& eul) {
			const T cr = T(std::cos(kl::math::toRads(eul.x) * 0.5));
			const T sr = T(std::sin(kl::math::toRads(eul.x) * 0.5));
			const T cp = T(std::cos(kl::math::toRads(eul.y) * 0.5));
			const T sp = T(std::sin(kl::math::toRads(eul.y) * 0.5));
			const T cy = T(std::cos(kl::math::toRads(eul.z) * 0.5));
			const T sy = T(std::sin(kl::math::toRads(eul.z) * 0.5));
			return {
				T(sr * cp * cy - cr * sp * sy),
				T(cr * sp * cy + sr * cp * sy),
				T(cr * cp * sy - sr * sp * cy),
				T(cr * cp * cy + sr * sp * sy)
			};
		}
		template<typename T> inline kl::vector3<T> toEul(const kl::vector4<T>& quat) {
			const T sinp = T(2.0 * (quat.w * quat.y - quat.z * quat.x));
			const T sinrCosp = T(2.0 * (quat.w * quat.x + quat.y * quat.z));
			const T cosrCosp = T(1.0 - 2.0 * (quat.x * quat.x + quat.y * quat.y));
			const T sinyCosp = T(2.0 * (quat.w * quat.z + quat.x * quat.y));
			const T cosyCosp = T(1.0 - 2.0 * (quat.y * quat.y + quat.z * quat.z));
			return {
				T(kl::math::toDegs(std::atan2(sinrCosp, cosrCosp))),
				T(kl::math::toDegs((std::abs(sinp) >= 1.0) ? std::copysign(kl::math::pi * 0.5, sinp) : std::asin(sinp))),
				T(kl::math::toDegs(std::atan2(sinyCosp, cosyCosp)))
			};
		}

		// minmax
		template<typename T> inline T minmax(const T& val, const T& min, const T& max) {
			return std::min(std::max(val, min), max);
		}

	};
}
