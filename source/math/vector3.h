#pragma once

#include "math/vector2.h"
#include "graphics/color.h"

#undef min
#undef max

namespace kl {
	inline float toFloatColor(byte val) {
		static const float conv = 1.0f / 255.0f;
		return val * conv;
	}
	inline byte toByteColor(float val) {
		return byte(std::min(std::max(val, 0.0f), 1.0f) * 255);
	}
}

namespace kl {
	template<typename T> struct vector3 {
		union {
			struct {
				T x, y, z;
			};
			kl::vector2<T> xy;
			T data[3] = {};
		};

		vector3() {}
		vector3(const T& a) : x(a), y(a), z(a) {}
		vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
		vector3(const kl::vector2<T>& v, const T& z) : x(v.x), y(v.y), z(z) {}
		vector3(const T& x, const kl::vector2<T>& v) : x(x), y(v.x), z(v.y) {}
		vector3(const kl::color& col) : x(toFloatColor(col.r)), y(toFloatColor(col.g)), z(toFloatColor(col.b)) {}
		template<typename V> vector3(const kl::vector3<V>& obj) : x(T(obj.x)), y(T(obj.y)), z(T(obj.z)) {}

		// Getters
		T& operator[](size_t ind) {
			return data[ind];
		}
		const T& operator[](size_t ind) const {
			return data[ind];
		}
		kl::color toColor() const {
			return kl::color(toByteColor(x), toByteColor(y), toByteColor(z));
		}

		// Addition
		void add(const kl::vector3<T>& obj, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] + obj[i];
			}
		}
		kl::vector3<T> operator+(const kl::vector3<T>& obj) const {
			kl::vector3<T> temp;
			add(obj, temp);
			return temp;
		}
		void operator+=(const kl::vector3<T>& obj) {
			add(obj, *this);
		}

		// Subtraction
		void sub(const kl::vector3<T>& obj, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] - obj[i];
			}
		}
		kl::vector3<T> operator-(const kl::vector3<T>& obj) const {
			kl::vector3<T> temp;
			sub(obj, temp);
			return temp;
		}
		void operator-=(const kl::vector3<T>& obj) {
			sub(obj, *this);
		}

		// Multiplication
		void mul(const T& val, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] * val;
			}
		}
		kl::vector3<T> operator*(const T& val) const {
			kl::vector3<T> temp;
			mul(val, temp);
			return temp;
		}
		void operator*=(const T& val) {
			mul(val, *this);
		}
		void mul(const kl::vector3<T>& obj, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] * obj[i];
			}
		}
		kl::vector3<T> operator*(const kl::vector3<T>& obj) const {
			kl::vector3<T> temp;
			mul(obj, temp);
			return temp;
		}
		void operator*=(const kl::vector3<T>& obj) {
			mul(obj, *this);
		}

		// Division
		void div(const T& val, kl::vector3<T>& out) const {
			const double recVal = 1.0 / val;
			for (int i = 0; i < 3; i++) {
				out[i] = T(data[i] * recVal);
			}
		}
		kl::vector3<T> operator/(const T& val) const {
			kl::vector3<T> temp;
			div(val, temp);
			return temp;
		}
		void operator/=(const T& val) {
			div(val, *this);
		}
		void div(const kl::vector3<T>& obj, kl::vector3<T>& out) const {
			for (int i = 0; i < 3; i++) {
				out[i] = data[i] / obj[i];
			}
		}
		kl::vector3<T> operator/(const kl::vector3<T>& obj) const {
			kl::vector3<T> temp;
			div(obj, temp);
			return temp;
		}
		void operator/=(const kl::vector3<T>& obj) {
			div(obj, *this);
		}

		// Conditional
		bool equ(const kl::vector3<T>& obj) const {
			for (int i = 0; i < 3; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		bool operator==(const kl::vector3<T>& obj) const {
			return equ(obj);
		}
		bool operator!=(const kl::vector3<T>& obj) const {
			return !equ(obj);
		}

		// Sign change
		void abs(kl::vector3<T>& out) const {
			for (size_t i = 0; i < 3; i++) {
				out[i] = std::abs(data[i]);
			}
		}
		kl::vector3<T> abs() const {
			kl::vector3<T> temp;
			abs(temp);
			return temp;
		}
		void neg(kl::vector3<T>& out) const {
			mul(-1.0f, out);
		}
		kl::vector3<T> neg() const {
			kl::vector3<T> temp;
			neg(temp);
			return temp;
		}

		// Length
		T len() const {
			T sum = {};
			for (size_t i = 0; i < 3; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		// Normalized
		void norm(kl::vector3<T>& out) const {
			out = (*this) / len();
		}
		kl::vector3<T> norm() const {
			kl::vector3<T> temp;
			norm(temp);
			return temp;
		}

		// Dot product
		T dot(const kl::vector3<T>& vec) const {
			T sum = {};
			for (size_t i = 0; i < 3; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		// Cross product
		kl::vector3<T> cross(const kl::vector3<T>& a) const {
			return kl::vector3<T>(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
		}

		// Angle between vectors
		T angle(const kl::vector3<T>& vec, bool full = false) const {
			return kl::math::toDegs(std::acos(norm().dot(vec.norm())));
		}

		// Rotate vector by around other vector
		kl::vector3<T> rotate(const T& angle, const kl::vector3<T>& axis) const {
			const T angleSin = T(std::sin(kl::math::toRads(angle) * 0.5));
			const T angleCos = T(std::cos(kl::math::toRads(angle) * 0.5));
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
