#pragma once

#include "Math/Vector3.h"


namespace kl {
	template<typename T>
	struct Vector4 {
		union {
			struct {
				T x, y, z, w;
			};
			struct {
				T r, g, b, a;
			};
			Vector2<T> xy;
			Vector3<T> xyz;
			Vector2<T> rg;
			Vector3<T> rgb;
			T data[4] = {};
		};

		Vector4() {}

		template<typename T0, typename T1, typename T2, typename T3>
		Vector4(const T0& x, const T1& y, const T2& z, const T3& w) : x(T(x)), y(T(y)), z(T(z)), w(T(w)) {}

		template<typename T0>
		Vector4(const Vector3<T>& v, const T0& w) : x(v.x), y(v.y), z(v.z), w(T(w)) {}

		template<typename T0>
		Vector4(const T0& x, const Vector3<T>& v) : x(T(x)), y(v.x), z(v.y), w(v.z) {}

		template<typename T0, typename T1>
		Vector4(const Vector2<T>& v, const T0& z, const T1& w) : x(v.x), y(v.y), z(T(z)), w(T(w)) {}

		template<typename T0, typename T1>
		Vector4(const T0& x, const Vector2<T>& v, const T1& w) : x(T(x)), y(v.x), z(v.y), w(T(w)) {}

		template<typename T0, typename T1>
		Vector4(const T0& x, const T1& y, const Vector2<T>& v) : x(T(x)), y(T(y)), z(v.x), w(v.y) {}

		Vector4(const Color& color) : x(ToFloatColor(color.r)), y(ToFloatColor(color.g)), z(ToFloatColor(color.b)), w(ToFloatColor(color.a)) {}

		T& operator[](int ind) {
			return data[ind];
		}

		const T& operator[](int ind) const {
			return data[ind];
		}

		template<typename T0>
		operator Vector4<T0>() const {
			return { T0(x), T0(y), T0(z), T0(w) };
		}

		operator Color() const {
			return { ToByteColor(x), ToByteColor(y), ToByteColor(z), ToByteColor(w) };
		}

		Vector4<T> operator+(const Vector4<T>& obj) const {
			Vector4<T> result;
			for (int i = 0; i < 4; i++) {
				result[i] = data[i] + obj[i];
			}
			return result;
		}

		void operator+=(const Vector4<T>& obj) {
			*this = *this + obj;
		}

		Vector4<T> operator-(const Vector4<T>& obj) const {
			Vector4<T> result;
			for (int i = 0; i < 4; i++) {
				result[i] = data[i] - obj[i];
			}
			return result;
		}

		void operator-=(const Vector4<T>& obj) {
			*this = *this - obj;
		}

		Vector4<T> operator*(const T& val) const {
			Vector4<T> result;
			for (int i = 0; i < 4; i++) {
				result[i] = data[i] * val;
			}
			return result;
		}

		void operator*=(const T& val) {
			*this = *this * val;
		}

		Vector4<T> operator*(const Vector4<T>& obj) const {
			Vector4<T> result;
			for (int i = 0; i < 4; i++) {
				result[i] = data[i] * obj[i];
			}
			return result;
		}

		void operator*=(const Vector4<T>& obj) {
			*this = *this * obj;
		}

		Vector4<T> operator/(const T& val) const {
			Vector4<T> result;
			for (int i = 0; i < 4; i++) {
				result[i] = data[i] / val;
			}
			return result;
		}

		void operator/=(const T& val) {
			*this = *this / val;
		}

		Vector4<T> operator/(const Vector4<T>& obj) const {
			Vector4<T> result;
			for (int i = 0; i < 4; i++) {
				result[i] = data[i] / obj[i];
			}
			return result;
		}

		void operator/=(const Vector4<T>& obj) {
			*this = *this / obj;
		}

		bool operator==(const Vector4<T>& obj) const {
			for (int i = 0; i < 4; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}

		bool operator!=(const Vector4<T>& obj) const {
			return !(*this == obj);
		}

		Vector4<T> absolute() const {
			Vector4<T> result;
			for (int i = 0; i < 4; i++) {
				result[i] = std::abs(data[i]);
			}
			return result;
		}

		Vector4<T> negate() const {
			return *this * -1.0f;
		}

		T length() const {
			T sum = {};
			for (int i = 0; i < 4; i++) {
				sum += data[i] * data[i];
			}
			return std::sqrt(sum);
		}

		Vector4<T> normalize() const {
			return *this / length();
		}

		T dot(const Vector4<T>& vec) const {
			T sum = {};
			for (int i = 0; i < 4; i++) {
				sum += data[i] * vec[i];
			}
			return sum;
		}

		T angle(const Vector4<T>& vec) const {
			return Math::ToDegrees(std::acos(normalize().dot(vec.normalize())));
		}
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& stream, const Vector4<T>& obj) {
		stream << std::fixed << std::setprecision(2);
		stream << "(" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << ")";
		return stream;
	}
}

namespace kl {
	using Float4 = Vector4<float>;
	using Double4 = Vector4<double>;
	using Int4 = Vector4<int>;
	using UInt4 = Vector4<uint>;
	using Bool4 = Vector4<bool>;

	using Vec4 = Float4;
	using DVec4 = Double4;
	using IVec4 = Int4;
	using UVec4 = UInt4;
	using BVec4 = Bool4;
}
