#pragma once

#include "Math/Matrix.h"
#include "Math/Vector3.h"


namespace kl {
	template<typename T>
	struct Matrix3x3 : public Matrix<T, 3, 3> {
		Matrix3x3() {}

		Matrix3x3(const Vector3<T>& r0, const Vector3<T>& r1, const Vector3<T>& r2) {
			(*this)[0] = r0.x; (*this)[1] = r0.y; (*this)[2] = r0.z;
			(*this)[3] = r1.x; (*this)[4] = r1.y; (*this)[5] = r1.z;
			(*this)[6] = r2.x; (*this)[7] = r2.y; (*this)[8] = r2.z;
		}

		Matrix3x3(const Matrix<T, 3, 3>& obj) : Matrix<T, 3, 3>(obj) {}

		static Matrix3x3<T> Translation(const Vector2<T>& val) {
			Matrix3x3<T> result;
			result[2] = val.x;
			result[5] = val.y;
			return result;
		}

		static Matrix3x3<T> Rotation(const T& val) {
			const T zSin = std::sin(Math::ToRadians(val));
			const T zCos = std::cos(Math::ToRadians(val));

			Matrix3x3<T> result;
			result[0] = zCos;
			result[1] = -zSin;
			result[3] = zSin;
			result[4] = zCos;
			return result;
		}

		static Matrix3x3<T> Scaling(const Vector2<T>& val) {
			Matrix3x3<T> result;
			result[0] = val.x;
			result[4] = val.y;
			return result;
		}
	};
}

namespace kl {
	using Float3x3 = Matrix3x3<float>;
	using Double3x3 = Matrix3x3<double>;
	using Int3x3 = Matrix3x3<int>;
	using UInt3x3 = Matrix3x3<uint>;
	using Bool3x3 = Matrix3x3<bool>;

	using Mat3 = Float3x3;
	using DMat3 = Double3x3;
	using IMat3 = Int3x3;
	using UMat3 = UInt3x3;
	using BMat3 = Bool3x3;
}
