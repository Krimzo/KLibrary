#pragma once

#include "math/vector2.h"
#include "graphics/color.h"


namespace kl {
	ALL inline float toFloatColor(byte val) {
		return val * 0.00392156862;
	}
	ALL inline byte toByteColor(float val) {
		return byte(min(max(val, 0.0f), 1.0f) * 255);
	}
}

namespace kl {
	template<typename T> struct vector3 {
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
			kl::vector2<T> xy;
			T data[3] = {};
		};

		ALL vector3() {}
		ALL vector3(const T& a) : x(a), y(a), z(a) {}
		ALL vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
		ALL vector3(const kl::vector2<T>& v, const T& z) : x(v.x), y(v.y), z(z) {}
		ALL vector3(const T& x, const kl::vector2<T>& v) : x(x), y(v.x), z(v.y) {}
		ALL vector3(const kl::color& col) : x(toFloatColor(col.r)), y(toFloatColor(col.g)), z(toFloatColor(col.b)) {}
		template<typename V> ALL vector3(const kl::vector3<V>& obj) : x(T(obj.x)), y(T(obj.y)), z(T(obj.z)) {}

		// Getters
		ALL T& operator[](uint64 ind) {
			return data[ind];
		}
		ALL const T& operator[](uint64 ind) const {
			return data[ind];
		}
		ALL kl::color toColor() const {
			return kl::color(toByteColor(x), toByteColor(y), toByteColor(z));
		}

		// Addition
		ALL void add(const kl::vector3<T>& obj, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] + obj[i];
			}
		}
		ALL kl::vector3<T> operator+(const kl::vector3<T>& obj) const {
			kl::vector3<T> temp;
			add(obj, temp);
			return temp;
		}
		ALL void operator+=(const kl::vector3<T>& obj) {
			add(obj, *this);
		}

		// Subtraction
		ALL void subtract(const kl::vector3<T>& obj, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] - obj[i];
			}
		}
		ALL kl::vector3<T> operator-(const kl::vector3<T>& obj) const {
			kl::vector3<T> temp;
			subtract(obj, temp);
			return temp;
		}
		ALL void operator-=(const kl::vector3<T>& obj) {
			subtract(obj, *this);
		}

		// Multiplication
		ALL void multiply(const T& val, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] * val;
			}
		}
		ALL kl::vector3<T> operator*(const T& val) const {
			kl::vector3<T> temp;
			multiply(val, temp);
			return temp;
		}
		ALL void operator*=(const T& val) {
			multiply(val, *this);
		}
		ALL void multiply(const kl::vector3<T>& obj, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] * obj[i];
			}
		}
		ALL kl::vector3<T> operator*(const kl::vector3<T>& obj) const {
			kl::vector3<T> temp;
			multiply(obj, temp);
			return temp;
		}
		ALL void operator*=(const kl::vector3<T>& obj) {
			multiply(obj, *this);
		}

		// Division
		ALL void divide(const T& val, kl::vector3<T>& out) const {
			const double recVal = 1.0 / val;
			for (int i = 0; i < 3; i++) {
				out[i] = T(data[i] * recVal);
			}
		}
		ALL kl::vector3<T> operator/(const T& val) const {
			kl::vector3<T> temp;
			divide(val, temp);
			return temp;
		}
		ALL void operator/=(const T& val) {
			divide(val, *this);
		}
		ALL void divide(const kl::vector3<T>& obj, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] / obj[i];
			}
		}
		ALL kl::vector3<T> operator/(const kl::vector3<T>& obj) const {
			kl::vector3<T> temp;
			divide(obj, temp);
			return temp;
		}
		ALL void operator/=(const kl::vector3<T>& obj) {
			divide(obj, *this);
		}

		// Conditional
		ALL bool equals(const kl::vector3<T>& obj) const {
			for (int i = 0; i < 3; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		ALL bool operator==(const kl::vector3<T>& obj) const {
			return equals(obj);
		}
		ALL bool operator!=(const kl::vector3<T>& obj) const {
			return !equals(obj);
		}

		// Sign change
		ALL void absolute(kl::vector3<T>& out) const {
			for (uint64 i = 0; i < 3; i++) {
				out[i] = std::abs(data[i]);
			}
		}
		ALL kl::vector3<T> absolute() const {
			kl::vector3<T> temp;
			absolute(temp);
			return temp;
		}
		ALL void negate(kl::vector3<T>& out) const {
			multiply(-1.0f, out);
		}
		ALL kl::vector3<T> negate() const {
			kl::vector3<T> temp;
			negate(temp);
			return temp;
		}

		// Length
		ALL T length() const {
			T sum = {};
			for (uint64 i = 0; i < 3; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		// Normalized
		ALL void normalize(kl::vector3<T>& out) const {
			out = (*this) / length();
		}
		ALL kl::vector3<T> normalize() const {
			kl::vector3<T> temp;
			normalize(temp);
			return temp;
		}

		// Dot product
		ALL T dot(const kl::vector3<T>& vec) const {
			T sum = {};
			for (uint64 i = 0; i < 3; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		// Cross product
		ALL kl::vector3<T> cross(const kl::vector3<T>& a) const {
			return kl::vector3<T>(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
		}

		// Angle between vectors
		ALL T angle(const kl::vector3<T>& vec, bool full = false) const {
			return kl::math::toDegrees(std::acos(normalize().dot(vec.normalize())));
		}

		// Rotate vector by around other vector
		ALL kl::vector3<T> rotate(const T& angle, const kl::vector3<T>& axis) const {
			const T angleSin = T(std::sin(kl::math::toRadians(angle) * 0.5));
			const T angleCos = T(std::cos(kl::math::toRadians(angle) * 0.5));
			const T qx = T(axis.x * angleSin);
			const T qy = T(axis.y * angleSin);
			const T qz = T(axis.z * angleSin);
			const T x2 = T(qx * qx);
			const T y2 = T(qy * qy);
			const T z2 = T(qz * qz);
			const T w2 = T(angleCos * angleCos);
			const T xy = T(qx * qy);
			const T xz = T(qx * qz);
			const T yz = T(qy * qz);
			const T xw = T(qx * angleCos);
			const T yw = T(qy * angleCos);
			const T zw = T(qz * angleCos);
			return {
				(w2 + x2 - z2 - y2) * x + (-zw + xy - zw + xy) * y + (yw + xz + xz + yw) * z,
				(xy + zw + zw + xy) * x + (y2 - z2 + w2 - x2) * y + (yz + yz - xw - xw) * z,
				(xz - yw + xz - yw) * x + (yz + yz + xw + xw) * y + (z2 - y2 - x2 + w2) * z
			};
		}

		// Reflect vector around other vector
		ALL kl::vector3<T> reflect(const kl::vector3<T>& vec) const {
			const kl::vector3<T> normal = vec.normalize();
			return subtract(normal * dot(normal) * 2.0);
		}
	};

	// std::cout
	template<typename T> inline std::ostream& operator<<(std::ostream& stream, const kl::vector3<T>& obj) {
		stream << std::fixed << std::setprecision(2);
		stream << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
		return stream;
	}
}

namespace kl {
	using float3 = kl::vector3<float>;
	using double3 = kl::vector3<double>;
	using int3 = kl::vector3<int>;
	using uint3 = kl::vector3<uint>;
	using bool3 = kl::vector3<bool>;

	using vec3 = float3;
	using dvec3 = double3;
	using ivec3 = int3;
	using uvec3 = uint3;
	using bvec3 = bool3;
}
