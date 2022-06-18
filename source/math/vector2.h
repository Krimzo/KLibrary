#pragma once

#include <iostream>
#include <iomanip>


using uint = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

namespace kl::math {
	inline const double pi = 3.14159265358979;

	template<typename T> inline T toRads(const T& degs) {
		static const double conv = kl::math::pi / 180.0;
		return T(degs * conv);
	}
	template<typename T> inline T toDegs(const T& rads) {
		static const double conv = 180.0 / kl::math::pi;
		return T(rads * conv);
	}
}

namespace kl {
	template<typename T> struct vector2 {
		union {
			struct {
				T x, y;
			};
			struct {
				T u, v;
			};
			struct {
				T width, height;
			};
			T data[2] = {};
		};

		vector2() {}
		vector2(const T& a) : x(a), y(a) {}
		vector2(const T& x, const T& y) : x(x), y(y) {}
		template<typename V> vector2(const kl::vector2<V>& obj) : x(T(obj.x)), y(T(obj.y)) {}

		// Getters
		T& operator[](uint64 ind) {
			return data[ind];
		}
		const T& operator[](uint64 ind) const {
			return data[ind];
		}

		// Addition
		void add(const kl::vector2<T>& obj, kl::vector2<T>& out) const {
			for (int i = 0; i < 2; i++) {
				out[i] = data[i] + obj[i];
			}
		}
		kl::vector2<T> operator+(const kl::vector2<T>& obj) const {
			kl::vector2<T> temp;
			add(obj, temp);
			return temp;
		}
		void operator+=(const kl::vector2<T>& obj) {
			add(obj, *this);
		}

		// Subtraction
		void sub(const kl::vector2<T>& obj, kl::vector2<T>& out) const {
			for (int i = 0; i < 2; i++) {
				out[i] = data[i] - obj[i];
			}
		}
		kl::vector2<T> operator-(const kl::vector2<T>& obj) const {
			kl::vector2<T> temp;
			sub(obj, temp);
			return temp;
		}
		void operator-=(const kl::vector2<T>& obj) {
			sub(obj, *this);
		}

		// Multiplication
		void mul(const T& val, kl::vector2<T>& out) const {
			for (int i = 0; i < 2; i++) {
				out[i] = data[i] * val;
			}
		}
		kl::vector2<T> operator*(const T& val) const {
			kl::vector2<T> temp;
			mul(val, temp);
			return temp;
		}
		void operator*=(const T& val) {
			mul(val, *this);
		}
		void mul(const kl::vector2<T>& obj, kl::vector2<T>& out) const {
			for (int i = 0; i < 2; i++) {
				out[i] = data[i] * obj[i];
			}
		}
		kl::vector2<T> operator*(const kl::vector2<T>& obj) const {
			kl::vector2<T> temp;
			mul(obj, temp);
			return temp;
		}
		void operator*=(const kl::vector2<T>& obj) {
			mul(obj, *this);
		}

		// Division
		void div(const T& val, kl::vector2<T>& out) const {
			const double recVal = 1.0 / val;
			for (int i = 0; i < 2; i++) {
				out[i] = T(data[i] * recVal);
			}
		}
		kl::vector2<T> operator/(const T& val) const {
			kl::vector2<T> temp;
			div(val, temp);
			return temp;
		}
		void operator/=(const T& val) {
			div(val, *this);
		}
		void div(const kl::vector2<T>& obj, kl::vector2<T>& out) const {
			for (int i = 0; i < 2; i++) {
				out[i] = data[i] / obj[i];
			}
		}
		kl::vector2<T> operator/(const kl::vector2<T>& obj) const {
			kl::vector2<T> temp;
			div(obj, temp);
			return temp;
		}
		void operator/=(const kl::vector2<T>& obj) {
			div(obj, *this);
		}

		// Conditional
		bool equ(const kl::vector2<T>& obj) const {
			for (int i = 0; i < 2; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		bool operator==(const kl::vector2<T>& obj) const {
			return equ(obj);
		}
		bool operator!=(const kl::vector2<T>& obj) const {
			return !equ(obj);
		}

		// Sign change
		void abs(kl::vector2<T>& out) const {
			for (uint64 i = 0; i < 2; i++) {
				out[i] = std::abs(data[i]);
			}
		}
		kl::vector2<T> abs() const {
			kl::vector2<T> temp;
			abs(temp);
			return temp;
		}
		void neg(kl::vector2<T>& out) const {
			mul(-1.0f, out);
		}
		kl::vector2<T> neg() const {
			kl::vector2<T> temp;
			neg(temp);
			return temp;
		}

		// Length
		T len() const {
			T sum = {};
			for (uint64 i = 0; i < 2; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		// Normalized
		void norm(kl::vector2<T>& out) const {
			out = (*this) / len();
		}
		kl::vector2<T> norm() const {
			kl::vector2<T> temp;
			norm(temp);
			return temp;
		}

		// Dot product
		T dot(const kl::vector2<T>& vec) const {
			T sum = {};
			for (uint64 i = 0; i < 2; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		// Angle between vectors
		T angle(const kl::vector2<T>& vec, bool full = false) const {
			return kl::math::toDegs(std::acos(norm().dot(vec.norm())));
		}

		// Rotate vector by angle
		kl::vector2<T> rotate(const T& angle) const {
			const T sinA = T(std::sin(kl::math::toRads(angle)));
			const T cosA = T(std::cos(kl::math::toRads(angle)));
			return {
				cosA * x - sinA * y,
				sinA * x + cosA * y
			};
		}
	};

	// std::cout
	template<typename T> inline std::ostream& operator<<(std::ostream& stream, const kl::vector2<T>& obj) {
		stream << std::fixed << std::setprecision(2);
		stream << "(" << obj.x << ", " << obj.y << ")";
		return stream;
	}
}

namespace kl {
	using float2 = kl::vector2<float>;
	using double2 = kl::vector2<double>;
	using int2 = kl::vector2<int>;
	using uint2 = kl::vector2<uint>;
	using bool2 = kl::vector2<bool>;

	using vec2 = float2;
	using dvec2 = double2;
	using ivec2 = int2;
	using uvec2 = uint2;
	using bvec2 = bool2;
}
