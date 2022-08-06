#pragma once

#include <iostream>
#include <iomanip>

#include "KLTypes.h"


namespace kl::Math {
	inline constexpr float PI = 3.14159265358979f;

	template<typename T>
	inline T ToRadians(const T& degs) {
		static const float conversion = PI / 180.0f;
		return T(degs * conversion);
	}

	template<typename T>
	inline T ToDegrees(const T& rads) {
		static const float conversion = 180.0f / PI;
		return T(rads * conversion);
	}
}

namespace kl {
	template<typename T>
	struct Vector2 {
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

		Vector2() {}

		template<typename T0, typename T1>
		Vector2(const T0& x, const T1& y) : x(T(x)), y(T(y)) {}

		T& operator[](int ind) {
			return data[ind];
		}

		const T& operator[](int ind) const {
			return data[ind];
		}

		template<typename T0>
		operator Vector2<T0>() const {
			return { T0(x), T0(y) };
		}

		Vector2<T> operator+(const Vector2<T>& obj) const {
			Vector2<T> result;
			for (int i = 0; i < 2; i++) {
				result[i] = data[i] + obj[i];
			}
			return result;
		}

		void operator+=(const Vector2<T>& obj) {
			*this = *this + obj;
		}

		Vector2<T> operator-(const Vector2<T>& obj) const {
			Vector2<T> result;
			for (int i = 0; i < 2; i++) {
				result[i] = data[i] - obj[i];
			}
			return result;
		}

		void operator-=(const Vector2<T>& obj) {
			*this = *this - obj;
		}

		Vector2<T> operator*(const T& val) const {
			Vector2<T> result;
			for (int i = 0; i < 2; i++) {
				result[i] = data[i] * val;
			}
			return result;
		}

		void operator*=(const T& val) {
			*this = *this * val;
		}

		Vector2<T> operator*(const Vector2<T>& obj) const {
			Vector2<T> result;
			for (int i = 0; i < 2; i++) {
				result[i] = data[i] * obj[i];
			}
			return result;
		}

		void operator*=(const Vector2<T>& obj) {
			*this = *this * obj;
		}

		Vector2<T> operator/(const T& val) const {
			Vector2<T> result;
			for (int i = 0; i < 2; i++) {
				result[i] = data[i] / val;
			}
			return result;
		}

		void operator/=(const T& val) {
			*this = *this / val;
		}

		Vector2<T> operator/(const Vector2<T>& obj) const {
			Vector2<T> result;
			for (int i = 0; i < 2; i++) {
				result[i] = data[i] / obj[i];
			}
			return result;
		}

		void operator/=(const Vector2<T>& obj) {
			*this = *this / obj;
		}

		bool operator==(const Vector2<T>& obj) const {
			for (int i = 0; i < 2; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}

		bool operator!=(const Vector2<T>& obj) const {
			return !(*this == obj);
		}

		Vector2<T> absolute() const {
			Vector2<T> result;
			for (int i = 0; i < 2; i++) {
				result[i] = std::abs(data[i]);
			}
			return result;
		}

		Vector2<T> negate() const {
			return *this * -1.0f;
		}

		T length() const {
			T sum = {};
			for (int i = 0; i < 2; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		Vector2<T> normalize() const {
			return *this / length();
		}

		T dot(const Vector2<T>& vec) const {
			T sum = {};
			for (int i = 0; i < 2; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		T angle(const Vector2<T>& vec, bool full = false) const {
			if (full) {
				return Math::ToDegrees(std::atan2(x * vec.y - y * vec.x, x * vec.x + y * vec.y));
			}
			return Math::ToDegrees(std::acos(normalize().dot(vec.normalize())));
		}

		Vector2<T> rotate(const T& angle) const {
			const T sinA = T(std::sin(Math::ToRadians(angle)));
			const T cosA = T(std::cos(Math::ToRadians(angle)));
			return { cosA * x - sinA * y, sinA * x + cosA * y };
		}
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& stream, const Vector2<T>& obj) {
		stream << std::fixed << std::setprecision(2);
		stream << "(" << obj.x << ", " << obj.y << ")";
		return stream;
	}
}

namespace kl {
	using Float2 = Vector2<float>;
	using Double2 = Vector2<double>;
	using Int2 = Vector2<int>;
	using UInt2 = Vector2<uint>;
	using Bool2 = Vector2<bool>;

	using Vec2 = Float2;
	using DVec2 = Double2;
	using IVec2 = Int2;
	using UVec2 = UInt2;
	using BVec2 = Bool2;
}
