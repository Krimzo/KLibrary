#pragma once

#include "math/vector3.h"


namespace kl {
	template<typename T> struct vector4 {
		union {
			struct {
				T x, y, z, w;
			};
			T data[4] = {};
		};

		vector4() {}
		vector4(const T& a) : x(a), y(a), z(a), w(a) {}
		vector4(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}
		vector4(const kl::vector3<T>& v, const T& w) : x(v.x), y(v.y), z(v.z), w(w) {}
		vector4(const T& x, const kl::vector3<T>& v) : x(x), y(v.x), z(v.y), w(v.z) {}
		vector4(const kl::vector2<T>& v, const T& z, const T& w) : x(v.x), y(v.y), z(z), w(w) {}
		vector4(const T& x, const kl::vector2<T>& v, const T& w) : x(x), y(v.x), z(v.y), w(w) {}
		vector4(const T& x, const T& y, const kl::vector2<T>& v) : x(x), y(y), z(v.x), w(v.y) {}
		vector4(const kl::color& col) : x(toFloatColor(col.r)), y(toFloatColor(col.g)), z(toFloatColor(col.b)), w(toFloatColor(col.a)) {}
		template<typename V> vector4(const kl::vector4<V>& obj) : x(T(obj.x)), y(T(obj.y)), z(T(obj.z)), w(T(obj.w)) {}

		// Getters
		T& operator[](size_t ind) {
			return data[ind];
		}
		const T& operator[](size_t ind) const {
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
		void sub(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] - obj[i];
			}
		}
		kl::vector4<T> operator-(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			sub(obj, temp);
			return temp;
		}
		void operator-=(const kl::vector4<T>& obj) {
			sub(obj, *this);
		}

		// Multiplication
		void mul(const T& val, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] * val;
			}
		}
		kl::vector4<T> operator*(const T& val) const {
			kl::vector4<T> temp;
			mul(val, temp);
			return temp;
		}
		void operator*=(const T& val) {
			mul(val, *this);
		}
		void mul(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] * obj[i];
			}
		}
		kl::vector4<T> operator*(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			mul(obj, temp);
			return temp;
		}
		void operator*=(const kl::vector4<T>& obj) {
			mul(obj, *this);
		}

		// Division
		void div(const T& val, kl::vector4<T>& out) const {
			const double recVal = 1.0 / val;
			for (int i = 0; i < 4; i++) {
				out[i] = T(data[i] * recVal);
			}
		}
		kl::vector4<T> operator/(const T& val) const {
			kl::vector4<T> temp;
			div(val, temp);
			return temp;
		}
		void operator/=(const T& val) {
			div(val, *this);
		}
		void div(const kl::vector4<T>& obj, kl::vector4<T>& out) const {
			for (int i = 0; i < 4; i++) {
				out[i] = data[i] / obj[i];
			}
		}
		kl::vector4<T> operator/(const kl::vector4<T>& obj) const {
			kl::vector4<T> temp;
			div(obj, temp);
			return temp;
		}
		void operator/=(const kl::vector4<T>& obj) {
			div(obj, *this);
		}

		// Conditional
		bool equ(const kl::vector4<T>& obj) const {
			for (int i = 0; i < 4; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		bool operator==(const kl::vector4<T>& obj) const {
			return equ(obj);
		}
		bool operator!=(const kl::vector4<T>& obj) const {
			return !equ(obj);
		}

		// Sign change
		void abs(kl::vector4<T>& out) const {
			for (size_t i = 0; i < 4; i++) {
				out[i] = std::abs(data[i]);
			}
		}
		kl::vector4<T> abs() const {
			kl::vector4<T> temp;
			abs(temp);
			return temp;
		}
		void neg(kl::vector4<T>& out) const {
			mul(-1.0f, out);
		}
		kl::vector4<T> neg() const {
			kl::vector4<T> temp;
			neg(temp);
			return temp;
		}

		// Length
		T len() const {
			T sum = {};
			for (size_t i = 0; i < 4; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		// Normalized
		void norm(kl::vector4<T>& out) const {
			out = (*this) / len();
		}
		kl::vector4<T> norm() const {
			kl::vector4<T> temp;
			norm(temp);
			return temp;
		}

		// Dot product
		T dot(const kl::vector4<T>& vec) const {
			T sum = {};
			for (size_t i = 0; i < 4; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		// Angle between vectors
		T angle(const kl::vector4<T>& vec, bool full = false) const {
			return kl::math::toDegs(std::acos(norm().dot(vec.norm())));
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
	using uint4 = kl::vector4<unsigned int>;
	using bool4 = kl::vector4<bool>;

	using vec4 = float4;
	using dvec4 = double4;
	using ivec4 = int4;
	using uvec4 = uint4;
	using bvec4 = bool4;
}
