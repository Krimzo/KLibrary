#pragma once

#include "Math/Matrix.h"
#include "Math/Vector4.h"


namespace kl {
	template<typename T>
	struct Matrix4x4 : public Matrix<T, 4, 4> {
		Matrix4x4() {
		}

		Matrix4x4(const Vector4<T>& r0, const Vector4<T>& r1, const Vector4<T>& r2, const Vector4<T>& r3) {
			(*this)[0] = r0.x; (*this)[1] = r0.y; (*this)[2] = r0.z; (*this)[3] = r0.w;
			(*this)[4] = r1.x; (*this)[5] = r1.y; (*this)[6] = r1.z; (*this)[7] = r1.w;
			(*this)[8] = r2.x; (*this)[9] = r2.y; (*this)[10] = r2.z; (*this)[11] = r2.w;
			(*this)[12] = r3.x; (*this)[13] = r3.y; (*this)[14] = r3.z; (*this)[15] = r3.w;
		}

		Matrix4x4(const Matrix<T, 4, 4>& mat) : Matrix<T, 4, 4>(mat) {
		}

		static Matrix4x4<T> Translation(const Vector3<T>& translation) {
			Matrix4x4<T> result;
			result[3] = translation.x;
			result[7] = translation.y;
			result[11] = translation.z;
			return result;
		}

		static Matrix4x4<T> Rotation(const Vector3<T>& rotation) {
			const T xRad = Math::ToRadians(rotation.x);
			const T xSin = std::sin(xRad);
			const T xCos = std::cos(xRad);

			Matrix4x4<T> xRot;
			xRot[5] = xCos;
			xRot[6] = -xSin;
			xRot[9] = xSin;
			xRot[10] = xCos;

			const T yRad = Math::ToRadians(rotation.y);
			const T ySin = std::sin(yRad);
			const T yCos = std::cos(yRad);

			Matrix4x4<T> yRot;
			yRot[0] = yCos;
			yRot[2] = ySin;
			yRot[8] = -ySin;
			yRot[10] = yCos;

			const T zRad = Math::ToRadians(rotation.z);
			const T zSin = std::sin(zRad);
			const T zCos = std::cos(zRad);

			Matrix4x4<T> zRot;
			zRot[0] = zCos;
			zRot[1] = -zSin;
			zRot[4] = zSin;
			zRot[5] = zCos;

			return zRot * yRot * xRot;
		}

		static Matrix4x4<T> Scaling(const Vector3<T>& scale) {
			Matrix4x4<T> result;
			result[0] = scale.x;
			result[5] = scale.y;
			result[10] = scale.z;
			return result;
		}

		static Matrix4x4<T> Perspective(T fieldOfView, T aspectRatio, T nearPlane, T farPlane) {
			const T tanHalf = T(1.0f / std::tan(Math::ToRadians(fieldOfView) * 0.5f));

			Matrix4x4<T> result;
			result[0] = tanHalf / aspectRatio;
			result[5] = tanHalf;
			result[10] = (-farPlane - nearPlane) / (nearPlane - farPlane);
			result[11] = T(2.0f * nearPlane * farPlane) / (nearPlane - farPlane);
			result[14] = T(1.0f);
			result[15] = T(0.0f);
			return result;
		}

		static Matrix4x4<T> Orthographic(T left, T right, T bottom, T top, T nearPlane, T farPlane) {
			Matrix4x4<T> result;
			result[0] = T(2.0f) / (right - left);
			result[5] = T(2.0f) / (top - bottom);
			result[10] = T(-2.0f) / (farPlane - nearPlane);
			result[3] = -(right + left) / (right - left);
			result[7] = -(top + bottom) / (top - bottom);
			result[11] = -(farPlane + nearPlane) / (farPlane - nearPlane);
			return result;
		}

		static Matrix4x4<T> LookAt(const Vector3<T>& position, const Vector3<T>& target, const Vector3<T>& up) {
			const Vector3<T> f = (target - position).normalize();
			const Vector3<T> s = up.cross(f).normalize();
			const Vector3<T> u = f.cross(s);

			Matrix4x4<T> result;
			result[0] = s.x;
			result[1] = s.y;
			result[2] = s.z;
			result[3] = -s.dot(position);
			result[4] = u.x;
			result[5] = u.y;
			result[6] = u.z;
			result[7] = -u.dot(position);
			result[8] = f.x;
			result[9] = f.y;
			result[10] = f.z;
			result[11] = -f.dot(position);
			return result;
		}
	};
}

namespace kl {
	using Float4x4 = Matrix4x4<float>;
	using Double4x4 = Matrix4x4<double>;
	using Int4x4 = Matrix4x4<int>;
	using UInt4x4 = Matrix4x4<uint>;
	using Bool4x4 = Matrix4x4<bool>;

	using Mat4 = Float4x4;
	using DMat4 = Double4x4;
	using IMat4 = Int4x4;
	using UMat4 = UInt4x4;
	using BMat4 = Bool4x4;
}
