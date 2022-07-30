#pragma once

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/matrix2x2.h"
#include "math/matrix3x3.h"
#include "math/matrix4x4.h"


namespace kl {
	namespace math {
		template<typename T = float, typename T0>
		inline T lineX(const kl::vector2<T>& a, const kl::vector2<T>& b, const T0& y) {
			return ((T(y) - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
		}

		template<typename T = float, typename T0>
		inline T lineY(const kl::vector2<T>& a, const kl::vector2<T>& b, const T0& x) {
			return ((b.y - a.y) * (T(x) - a.x)) / (b.x - a.x) + a.y;
		}

		template<typename T = float, typename T0, typename T1, typename T2>
		inline T normalize(const T0& val, const T1& lower, const T2& upper) {
			return (T(val) - T(lower)) / (T(upper) - T(lower));
		}

		template<typename T, typename T0, typename T1>
		inline T minmax(const T& val, const T0& min, const T1& max) {
			return std::min(std::max(val, T(min)), T(max));
		}
	};

	namespace to {
		template<typename T>
		inline kl::vector4<T> quaternion(const kl::vector3<T>& eul) {
			const T cr = T(std::cos(kl::to::radians(eul.x) * 0.5f));
			const T sr = T(std::sin(kl::to::radians(eul.x) * 0.5f));
			const T cp = T(std::cos(kl::to::radians(eul.y) * 0.5f));
			const T sp = T(std::sin(kl::to::radians(eul.y) * 0.5f));
			const T cy = T(std::cos(kl::to::radians(eul.z) * 0.5f));
			const T sy = T(std::sin(kl::to::radians(eul.z) * 0.5f));
			return {
				sr * cp * cy - cr * sp * sy,
				cr * sp * cy + sr * cp * sy,
				cr * cp * sy - sr * sp * cy,
				cr * cp * cy + sr * sp * sy
			};
		}

		template<typename T>
		inline kl::vector3<T> euler(const kl::vector4<T>& quat) {
			const T sinp = T(2.0f * (quat.w * quat.y - quat.z * quat.x));
			const T sinrCosp = T(2.0f * (quat.w * quat.x + quat.y * quat.z));
			const T cosrCosp = T(1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y));
			const T sinyCosp = T(2.0f * (quat.w * quat.z + quat.x * quat.y));
			const T cosyCosp = T(1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z));
			return {
				kl::to::degrees<T>(std::atan2(sinrCosp, cosrCosp)),
				kl::to::degrees<T>((std::abs(sinp) >= 1.0f) ? std::copysign(kl::math::pi * 0.5f, sinp) : std::asin(sinp)),
				kl::to::degrees<T>(std::atan2(sinyCosp, cosyCosp))
			};
		}
	}
}
