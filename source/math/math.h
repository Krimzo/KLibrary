#pragma once

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/matrix2x2.h"
#include "math/matrix3x3.h"
#include "math/matrix4x4.h"


namespace kl {
	namespace math {
		template<typename T = float, typename T0, typename T1, typename T2> inline T lineX(const kl::vector2<T0>& a, const kl::vector2<T1>& b, const T2& y) {
			return ((T(y) - T(a.y)) * (T(b.x) - T(a.x))) / (T(b.y) - T(a.y)) + T(a.x);
		}

		template<typename T = float, typename T0, typename T1, typename T2> inline T lineY(const kl::vector2<T0>& a, const kl::vector2<T1>& b, const T2& x) {
			return ((T(b.y) - T(a.y)) * (T(x) - T(a.x))) / (T(b.x) - T(a.x)) + T(a.y);
		}

		template<typename T = float, typename T0, typename T1, typename T2> inline T normalize(const T0& val, const T1& lower, const T2& upper) {
			return (T(val) - T(lower)) / (T(upper) - T(lower));
		}

		template<typename T, typename T0, typename T1> inline T minmax(const T& val, const T0& min, const T1& max) {
			return std::min(std::max(val, T(min)), T(max));
		}
	};

	namespace to {
		template<typename T> inline kl::vector4<T> quaternion(const kl::vector3<T>& eul) {
			const T cr = T(std::cos(kl::to::radians(eul.x) * 0.5));
			const T sr = T(std::sin(kl::to::radians(eul.x) * 0.5));
			const T cp = T(std::cos(kl::to::radians(eul.y) * 0.5));
			const T sp = T(std::sin(kl::to::radians(eul.y) * 0.5));
			const T cy = T(std::cos(kl::to::radians(eul.z) * 0.5));
			const T sy = T(std::sin(kl::to::radians(eul.z) * 0.5));
			return {
				T(sr * cp * cy - cr * sp * sy),
				T(cr * sp * cy + sr * cp * sy),
				T(cr * cp * sy - sr * sp * cy),
				T(cr * cp * cy + sr * sp * sy)
			};
		}
		template<typename T> inline kl::vector3<T> euler(const kl::vector4<T>& quat) {
			const T sinp = T(2.0 * (quat.w * quat.y - quat.z * quat.x));
			const T sinrCosp = T(2.0 * (quat.w * quat.x + quat.y * quat.z));
			const T cosrCosp = T(1.0 - 2.0 * (quat.x * quat.x + quat.y * quat.y));
			const T sinyCosp = T(2.0 * (quat.w * quat.z + quat.x * quat.y));
			const T cosyCosp = T(1.0 - 2.0 * (quat.y * quat.y + quat.z * quat.z));
			return {
				T(kl::to::degrees(std::atan2(sinrCosp, cosrCosp))),
				T(kl::to::degrees((std::abs(sinp) >= 1.0) ? std::copysign(kl::math::pi * 0.5, sinp) : std::asin(sinp))),
				T(kl::to::degrees(std::atan2(sinyCosp, cosyCosp)))
			};
		}
	}
}
