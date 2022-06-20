#pragma once

#include "math/vector3.h"


namespace kl {
	template<typename T> struct vector4 {
		union {
			struct {
				T x, y, z, w;
			};
			struct {
				T r, g, b, a;
			};
			kl::vector2<T> xy;
			kl::vector3<T> xyz;
			T data[4] = {};
		};

		ALL vector4() {}
		ALL vector4(const T& a) : x(a), y(a), z(a), w(a) {}
		ALL vector4(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}
		ALL vector4(const kl::vector3<T>& v, const T& w) : x(v.x), y(v.y), z(v.z), w(w) {}
		ALL vector4(const T& x, const kl::vector3<T>& v) : x(x), y(v.x), z(v.y), w(v.z) {}
		ALL vector4(const kl::vector2<T>& v, const T& z, const T& w) : x(v.x), y(v.y), z(z), w(w) {}
		ALL vector4(const T& x, const kl::vector2<T>& v, const T& w) : x(x), y(v.x), z(v.y), w(w) {}
		ALL vector4(const T& x, const T& y, const kl::vector2<T>& v) : x(x), y(y), z(v.x), w(v.y) {}
		ALL vector4(const kl::color& col) : x(toFloatColor(col.r)), y(toFloatColor(col.g)), z(toFloatColor(col.b)), w(toFloatColor(col.a)) {}
		template<typename V> ALL vector4(const kl::vector4<V>& obj) : x(T(obj.x)), y(T(obj.y)), z(T(obj.z)), w(T(obj.w)) {}

		// Getters
		ALL T& operator[](uint64 ind) {
			return data[ind];
		}
		ALL const T& operator[](uint64 ind) const {
			return data[ind];
		}
		ALL kl::color toColor() const {
			return kl::color(toByteColor(x), toByteColor(y), toByteColor(z), toByteColor(w));
		}

		// Addition
		ALL void add(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] + obj[i];
			}
		}
		ALL kl::vector4<T> operator+(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			add(obj, temp);
			return temp;
		}
		ALL void operator+=(const kl::vector4<T>& obj) {
			add(obj, *this);
		}

		// Subtraction
		ALL void subtract(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] - obj[i];
			}
		}
		ALL kl::vector4<T> operator-(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			subtract(obj, temp);
			return temp;
		}
		ALL void operator-=(const kl::vector4<T>& obj) {
			subtract(obj, *this);
		}

		// Multiplication
		ALL void multiply(const T& val, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] * val;
			}
		}
		ALL kl::vector4<T> operator*(const T& val) const {
			kl::vector4<T> temp;
			multiply(val, temp);
			return temp;
		}
		ALL void operator*=(const T& val) {
			multiply(val, *this);
		}
		ALL void multiply(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] * obj[i];
			}
		}
		ALL kl::vector4<T> operator*(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			multiply(obj, temp);
			return temp;
		}
		ALL void operator*=(const kl::vector4<T>& obj) {
			multiply(obj, *this);
		}

		// Division
		ALL void divide(const T& val, kl::vector4<T>& out) const {
			const double recVal = 1.0 / val;
			for (int i = 0; i < 4; i++) {
				out[i] = T(data[i] * recVal);
			}
		}
		ALL kl::vector4<T> operator/(const T& val) const {
			kl::vector4<T> temp;
			divide(val, temp);
			return temp;
		}
		ALL void operator/=(const T& val) {
			divide(val, *this);
		}
		ALL void divide(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] / obj[i];
			}
		}
		ALL kl::vector4<T> operator/(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			divide(obj, temp);
			return temp;
		}
		ALL void operator/=(const kl::vector4<T>& obj) {
			divide(obj, *this);
		}

		// Conditional
		ALL bool equals(const kl::vector4<T>& obj) const {
			for (int i = 0; i < 4; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		ALL bool operator==(const kl::vector4<T>& obj) const {
			return equals(obj);
		}
		ALL bool operator!=(const kl::vector4<T>& obj) const {
			return !equals(obj);
		}

		// Sign change
		ALL void absolute(kl::vector4<T>& out) const {
			for (uint64 i = 0; i < 4; i++) {
				out[i] = std::abs(data[i]);
			}
		}
		ALL kl::vector4<T> absolute() const {
			kl::vector4<T> temp;
			absolute(temp);
			return temp;
		}
		ALL void negate(kl::vector4<T>& out) const {
			multiply(-1.0f, out);
		}
		ALL kl::vector4<T> negate() const {
			kl::vector4<T> temp;
			negate(temp);
			return temp;
		}

		// Length
		ALL T length() const {
			T sum = {};
			for (uint64 i = 0; i < 4; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		// Normalized
		ALL void normalize(kl::vector4<T>& out) const {
			out = (*this) / length();
		}
		ALL kl::vector4<T> normalize() const {
			kl::vector4<T> temp;
			normalize(temp);
			return temp;
		}

		// Dot product
		ALL T dot(const kl::vector4<T>& vec) const {
			T sum = {};
			for (uint64 i = 0; i < 4; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		// Angle between vectors
		ALL T angle(const kl::vector4<T>& vec, bool full = false) const {
			return kl::math::toDegrees(std::acos(normalize().dot(vec.normalize())));
		}
	};

	// std::cout
	template<typename T> inline std::ostream& operator<<(std::ostream& stream, const kl::vector4<T>& obj) {
		stream << std::fixed << std::setprecision(2);
		stream << "(" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << ")";
		return stream;
	}
}

namespace kl {
	using float4 = kl::vector4<float>;
	using double4 = kl::vector4<double>;
	using int4 = kl::vector4<int>;
	using uint4 = kl::vector4<uint>;
	using bool4 = kl::vector4<bool>;

	using vec4 = float4;
	using dvec4 = double4;
	using ivec4 = int4;
	using uvec4 = uint4;
	using bvec4 = bool4;
}
