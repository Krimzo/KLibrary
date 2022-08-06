#pragma once

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/matrix2x2.h"
#include "math/matrix3x3.h"
#include "math/matrix4x4.h"


namespace kl {
	namespace Math {
		inline float GetLineX(const Float2& a, const Float2& b, float y) {
			return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
		}

		inline float GetLineY(const Float2& a, const Float2& b, float x) {
			return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
		}

		template<typename T = float, typename T0, typename T1, typename T2>
		inline T Normalize(const T0& val, const T1& lower, const T2& upper) {
			return (T(val) - T(lower)) / (T(upper) - T(lower));
		}

		template<typename T, typename T0, typename T1>
		inline T MinMax(const T& val, const T0& min, const T1& max) {
			return std::min(std::max(val, T(min)), T(max));
		}
	};

	template<typename T>
	inline kl::Vector4<T> ToQuaternion(const kl::Vector3<T>& eul) {
		const T cr = T(std::cos(Math::ToRadians(eul.x) * 0.5f));
		const T sr = T(std::sin(Math::ToRadians(eul.x) * 0.5f));
		const T cp = T(std::cos(Math::ToRadians(eul.y) * 0.5f));
		const T sp = T(std::sin(Math::ToRadians(eul.y) * 0.5f));
		const T cy = T(std::cos(Math::ToRadians(eul.z) * 0.5f));
		const T sy = T(std::sin(Math::ToRadians(eul.z) * 0.5f));
		return {
			sr * cp * cy - cr * sp * sy,
			cr * sp * cy + sr * cp * sy,
			cr * cp * sy - sr * sp * cy,
			cr * cp * cy + sr * sp * sy
		};
	}

	template<typename T>
	inline kl::Vector3<T> ToEuler(const Vector4<T>& quat) {
		const T sinp = T(2.0f * (quat.w * quat.y - quat.z * quat.x));
		const T sinrCosp = T(2.0f * (quat.w * quat.x + quat.y * quat.z));
		const T cosrCosp = T(1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y));
		const T sinyCosp = T(2.0f * (quat.w * quat.z + quat.x * quat.y));
		const T cosyCosp = T(1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z));
		return {
			Math::ToDegrees<T>(std::atan2(sinrCosp, cosrCosp)),
			Math::ToDegrees<T>((std::abs(sinp) >= 1.0f) ? std::copysign(Math::PI * 0.5f, sinp) : std::asin(sinp)),
			Math::ToDegrees<T>(std::atan2(sinyCosp, cosyCosp))
		};
	}
}
