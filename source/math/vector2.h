#pragma once

#include <iostream>
#include <iomanip>


using uint = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

namespace kl::math {
	inline const double pi = 3.14159265358979;

	template<typename T> inline T toRadians(const T& degs) {
		static const double conv = kl::math::pi / 180.0;
		return T(degs * conv);
	}
	template<typename T> inline T toDegrees(const T& rads) {
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
		void subtract(const kl::vector2<T>& obj, kl::vector2<T>& out) const {
			for (int i = 0; i < 2; i++) {
				out[i] = data[i] - obj[i];
			}
		}
		kl::vector2<T> operator-(const kl::vector2<T>& obj) const {
			kl::vector2<T> temp;
			subtract(obj, temp);
			return temp;
		}
		void operator-=(const kl::vector2<T>& obj) {
			subtract(obj, *this);
		}

		// Multiplication
		void multiply(const T& val, kl::vector2<T>& out) const {
			for (int i = 0; i < 2; i++) {
				out[i] = data[i] * val;
			}
		}
		kl::vector2<T> operator*(const T& val) const {
			kl::vector2<T> temp;
			multiply(val, temp);
			return temp;
		}
		void operator*=(const T& val) {
			multiply(val, *this);
		}
		void multiply(const kl::vector2<T>& obj, kl::vector2<T>& out) const {
			for (int i = 0; i < 2; i++) {
				out[i] = data[i] * obj[i];
			}
		}
		kl::vector2<T> operator*(const kl::vector2<T>& obj) const {
			kl::vector2<T> temp;
			multiply(obj, temp);
			return temp;
		}
		void operator*=(const kl::vector2<T>& obj) {
			multiply(obj, *this);
		}

		// Division
		void divide(const T& val, kl::vector2<T>& out) const {
			const double recVal = 1.0 / val;
			for (int i = 0; i < 2; i++) {
				out[i] = T(data[i] * recVal);
			}
		}
		kl::vector2<T> operator/(const T& val) const {
			kl::vector2<T> temp;
			divide(val, temp);
			return temp;
		}
		void operator/=(const T& val) {
			divide(val, *this);
		}
		void divide(const kl::vector2<T>& obj, kl::vector2<T>& out) const {
			for (int i = 0; i < 2; i++) {
				out[i] = data[i] / obj[i];
			}
		}
		kl::vector2<T> operator/(const kl::vector2<T>& obj) const {
			kl::vector2<T> temp;
			divide(obj, temp);
			return temp;
		}
		void operator/=(const kl::vector2<T>& obj) {
			divide(obj, *this);
		}

		// Conditional
		bool equals(const kl::vector2<T>& obj) const {
			for (int i = 0; i < 2; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		bool operator==(const kl::vector2<T>& obj) const {
			return equals(obj);
		}
		bool operator!=(const kl::vector2<T>& obj) const {
			return !equals(obj);
		}

		// Sign change
		void absolute(kl::vector2<T>& out) const {
			for (uint64 i = 0; i < 2; i++) {
				out[i] = std::abs(data[i]);
			}
		}
		kl::vector2<T> absolute() const {
			kl::vector2<T> temp;
			abs(temp);
			return temp;
		}
		void negate(kl::vector2<T>& out) const {
			multiply(-1.0f, out);
		}
		kl::vector2<T> negate() const {
			kl::vector2<T> temp;
			negate(temp);
			return temp;
		}

		// Length
		T length() const {
			T sum = {};
			for (uint64 i = 0; i < 2; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		// Normalized
		void normalize(kl::vector2<T>& out) const {
			out = (*this) / length();
		}
		kl::vector2<T> normalize() const {
			kl::vector2<T> temp;
			normalize(temp);
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
			if (full) {
				return kl::math::toDegrees(std::atan2(x * vec.y - y * vec.x, x * vec.x + y * vec.y));
			}
			return kl::math::toDegrees(std::acos(normalize().dot(vec.normalize())));
		}

		// Rotate vector by angle
		kl::vector2<T> rotate(const T& angle) const {
			const T sinA = T(std::sin(kl::math::toRadians(angle)));
			const T cosA = T(std::cos(kl::math::toRadians(angle)));
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
