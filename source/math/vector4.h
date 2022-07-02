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
			kl::vector2<T> rg;
			kl::vector3<T> rgb;
			T data[4] = {};
		};

		vector4() {}
		template<typename T0> vector4(const T0& a) : x(T(a)), y(T(a)), z(T(a)), w(T(a)) {}
		template<typename T0, typename T1, typename T2, typename T3> vector4(const T0& x, const T1& y, const T2& z, const T3& w) : x(T(x)), y(T(y)), z(T(z)), w(T(w)) {}
		template<typename T0, typename T1> vector4(const kl::vector3<T0>& v, const T1& w) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(w)) {}
		template<typename T0, typename T1> vector4(const T0& x, const kl::vector3<T1>& v) : x(T(x)), y(T(v.x)), z(T(v.y)), w(T(v.z)) {}
		template<typename T0, typename T1, typename T2> vector4(const kl::vector2<T0>& v, const T1& z, const T2& w) : x(T(v.x)), y(T(v.y)), z(T(z)), w(T(w)) {}
		template<typename T0, typename T1, typename T2> vector4(const T0& x, const kl::vector2<T1>& v, const T2& w) : x(T(x)), y(T(v.x)), z(T(v.y)), w(T(w)) {}
		template<typename T0, typename T1, typename T2> vector4(const T0& x, const T1& y, const kl::vector2<T2>& v) : x(T(x)), y(T(y)), z(T(v.x)), w(T(v.y)) {}
		template<typename T0> vector4(const kl::vector4<T0>& obj) : x(T(obj.x)), y(T(obj.y)), z(T(obj.z)), w(T(obj.w)) {}
		vector4(const kl::color& col) : x(toFloatColor(col.r)), y(toFloatColor(col.g)), z(toFloatColor(col.b)), w(toFloatColor(col.a)) {}

		// Getters
		T& operator[](uint64 ind) {
			return data[ind];
		}
		const T& operator[](uint64 ind) const {
			return data[ind];
		}
		kl::color toColor() const {
			return kl::color(toByteColor(x), toByteColor(y), toByteColor(z), toByteColor(w));
		}

		// Addition
		void add(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] + obj[i];
			}
		}
		kl::vector4<T> operator+(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			add(obj, temp);
			return temp;
		}
		void operator+=(const kl::vector4<T>& obj) {
			add(obj, *this);
		}

		// Subtraction
		void subtract(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] - obj[i];
			}
		}
		kl::vector4<T> operator-(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			subtract(obj, temp);
			return temp;
		}
		void operator-=(const kl::vector4<T>& obj) {
			subtract(obj, *this);
		}

		// Multiplication
		void multiply(const T& val, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] * val;
			}
		}
		kl::vector4<T> operator*(const T& val) const {
			kl::vector4<T> temp;
			multiply(val, temp);
			return temp;
		}
		void operator*=(const T& val) {
			multiply(val, *this);
		}
		void multiply(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] * obj[i];
			}
		}
		kl::vector4<T> operator*(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			multiply(obj, temp);
			return temp;
		}
		void operator*=(const kl::vector4<T>& obj) {
			multiply(obj, *this);
		}

		// Division
		void divide(const T& val, kl::vector4<T>& out) const {
			const double recVal = 1.0 / val;
			for (int i = 0; i < 4; i++) {
				out[i] = T(data[i] * recVal);
			}
		}
		kl::vector4<T> operator/(const T& val) const {
			kl::vector4<T> temp;
			divide(val, temp);
			return temp;
		}
		void operator/=(const T& val) {
			divide(val, *this);
		}
		void divide(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] / obj[i];
			}
		}
		kl::vector4<T> operator/(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			divide(obj, temp);
			return temp;
		}
		void operator/=(const kl::vector4<T>& obj) {
			divide(obj, *this);
		}

		// Conditional
		bool equals(const kl::vector4<T>& obj) const {
			for (int i = 0; i < 4; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		bool operator==(const kl::vector4<T>& obj) const {
			return equals(obj);
		}
		bool operator!=(const kl::vector4<T>& obj) const {
			return !equals(obj);
		}

		// Sign change
		void absolute(kl::vector4<T>& out) const {
			for (uint64 i = 0; i < 4; i++) {
				out[i] = std::abs(data[i]);
			}
		}
		kl::vector4<T> absolute() const {
			kl::vector4<T> temp;
			absolute(temp);
			return temp;
		}
		void negate(kl::vector4<T>& out) const {
			multiply(-1.0f, out);
		}
		kl::vector4<T> negate() const {
			kl::vector4<T> temp;
			negate(temp);
			return temp;
		}

		// Length
		T length() const {
			T sum = {};
			for (uint64 i = 0; i < 4; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		// Normalized
		void normalize(kl::vector4<T>& out) const {
			out = (*this) / length();
		}
		kl::vector4<T> normalize() const {
			kl::vector4<T> temp;
			normalize(temp);
			return temp;
		}

		// Dot product
		T dot(const kl::vector4<T>& vec) const {
			T sum = {};
			for (uint64 i = 0; i < 4; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		// Angle between vectors
		T angle(const kl::vector4<T>& vec) const {
			return kl::to::degrees(std::acos(normalize().dot(vec.normalize())));
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
