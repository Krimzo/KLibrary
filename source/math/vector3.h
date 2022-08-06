#pragma once

#include "math/vector2.h"
#include "graphics/color.h"


#undef min
#undef max

namespace kl {
	inline float ToFloatColor(byte val) {
		static const float conv = 1.0f / 255.0f;
		return val * conv;
	}

	inline byte ToByteColor(float val) {
		return byte(std::min(std::max(val, 0.0f), 1.0f) * 255);
	}
}

namespace kl {
	template<typename T>
	struct Vector3 {
		union {
			struct {
				T x, y, z;
			};
			struct {
				T r, g, b;
			};
			struct {
				T width, height, depth;
			};
			Vector2<T> xy;
			Vector2<T> rg;
			Vector2<T> wh;
			T data[3] = {};
		};

		Vector3() {}

		template<typename T0, typename T1, typename T2>
		Vector3(const T0& x, const T1& y, const T2& z) : x(T(x)), y(T(y)), z(T(z)) {}

		template<typename T0>
		Vector3(const Vector2<T>& v, const T0& z) : x(v.x), y(v.y), z(T(z)) {}

		template<typename T0>
		Vector3(const T0& x, const Vector2<T>& v) : x(T(x)), y(v.x), z(v.y) {}

		Vector3(const Color& color) : x(ToFloatColor(color.r)), y(ToFloatColor(color.g)), z(ToFloatColor(color.b)) {}

		T& operator[](int ind) {
			return data[ind];
		}

		const T& operator[](int ind) const {
			return data[ind];
		}

		template<typename T0>
		operator Vector3<T0>() const {
			return { T0(x), T0(y), T0(z) };
		}

		operator Color() const {
			return { ToByteColor(x), ToByteColor(y), ToByteColor(z) };
		}

		Vector3<T> operator+(const Vector3<T>& obj) const {
			Vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] + obj[i];
			}
			return result;
		}

		void operator+=(const kl::Vector3<T>& obj) {
			*this = *this + obj;
		}

		Vector3<T> operator-(const Vector3<T>& obj) const {
			Vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] - obj[i];
			}
			return result;
		}

		void operator-=(const kl::Vector3<T>& obj) {
			*this = *this - obj;
		}

		Vector3<T> operator*(const T& val) const {
			Vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] * val;
			}
			return result;
		}

		void operator*=(const T& val) {
			*this = *this * val;
		}

		Vector3<T> operator*(const Vector3<T>& obj) const {
			Vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] * obj[i];
			}
			return result;
		}

		void operator*=(const kl::Vector3<T>& obj) {
			*this = *this * obj;
		}

		Vector3<T> operator/(const T& val) const {
			Vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] / val;
			}
			return result;
		}

		void operator/=(const T& val) {
			*this = *this / val;
		}

		Vector3<T> operator/(const Vector3<T>& obj) const {
			Vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = data[i] / obj[i];
			}
			return result;
		}

		void operator/=(const Vector3<T>& obj) {
			*this = *this / obj;
		}

		bool operator==(const Vector3<T>& obj) const {
			for (int i = 0; i < 3; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}

		bool operator!=(const Vector3<T>& obj) const {
			return !(*this == obj);
		}

		Vector3<T> absolute() const {
			Vector3<T> result;
			for (int i = 0; i < 3; i++) {
				result[i] = std::abs(data[i]);
			}
			return result;
		}

		Vector3<T> negate() const {
			return *this * -1.0f;
		}

		T length() const {
			T sum = {};
			for (int i = 0; i < 3; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		Vector3<T> normalize() const {
			return *this / length();
		}

		T dot(const Vector3<T>& vec) const {
			T sum = {};
			for (int i = 0; i < 3; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		Vector3<T> cross(const Vector3<T>& a) const {
			return { y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x };
		}

		T angle(const Vector3<T>& vec) const {
			return Math::ToDegrees(std::acos(normalize().dot(vec.normalize())));
		}

		Vector3<T> rotate(const T& angle, const Vector3<T>& axis) const {
			const T angleSin = T(std::sin(Math::ToRadians(angle) * 0.5f));
			const T angleCos = T(std::cos(Math::ToRadians(angle) * 0.5f));
			const T qx = axis.x * angleSin;
			const T qy = axis.y * angleSin;
			const T qz = axis.z * angleSin;
			const T x2 = qx * qx;
			const T y2 = qy * qy;
			const T z2 = qz * qz;
			const T w2 = angleCos * angleCos;
			const T xy = qx * qy;
			const T xz = qx * qz;
			const T yz = qy * qz;
			const T xw = qx * angleCos;
			const T yw = qy * angleCos;
			const T zw = qz * angleCos;

			return {
				x * (w2 + x2 - z2 - y2) + y * (-zw + xy - zw + xy) + z * (yw + xz + xz + yw),
				x * (xy + zw + zw + xy) + y * (y2 - z2 + w2 - x2) + z * (yz + yz - xw - xw),
				x * (xz - yw + xz - yw) + y * (yz + yz + xw + xw) + z * (z2 - y2 - x2 + w2)
			};
		}

		Vector3<T> reflect(const Vector3<T>& vec) const {
			const Vector3<T> normal = vec.normalize();
			return *this - normal * dot(normal) * 2.0f;
		}
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& stream, const kl::Vector3<T>& obj) {
		stream << std::fixed << std::setprecision(2);
		stream << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
		return stream;
	}
}

namespace kl {
	using Float3 = Vector3<float>;
	using Double3 = Vector3<double>;
	using Int3 = Vector3<int>;
	using UInt3 = Vector3<uint>;
	using Bool3 = Vector3<bool>;

	using Vec3 = Float3;
	using DVec3 = Double3;
	using IVec3 = Int3;
	using UVec3 = UInt3;
	using BVec3 = Bool3;
}
