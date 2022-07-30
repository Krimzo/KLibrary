#pragma once

#include "math/matrix.h"
#include "math/vector4.h"


namespace kl
{
	template<typename T>
	struct matrix4x4 : public kl::matrix<T, 4, 4>
	{
		matrix4x4()
		{
		}
		matrix4x4(const kl::vector4<T>& r0, const kl::vector4<T>& r1, const kl::vector4<T>& r2, const kl::vector4<T>& r3)
		{
			(*this)[0] = r0.x; (*this)[1] = r0.y; (*this)[2] = r0.z; (*this)[3] = r0.w;
			(*this)[4] = r1.x; (*this)[5] = r1.y; (*this)[6] = r1.z; (*this)[7] = r1.w;
			(*this)[8] = r2.x; (*this)[9] = r2.y; (*this)[10] = r2.z; (*this)[11] = r2.w;
			(*this)[12] = r3.x; (*this)[13] = r3.y; (*this)[14] = r3.z; (*this)[15] = r3.w;
		}
		matrix4x4(const kl::matrix<T, 4, 4>& obj) : kl::matrix<T, 4, 4>(obj)
		{
		}

		// Translation matrix
		static kl::matrix4x4<T> translation(const kl::vector3<T>& val)
		{
			kl::matrix4x4<T> temp;
			temp[3] = val.x;
			temp[7] = val.y;
			temp[11] = val.z;
			return temp;
		}

		// Rotation matrix
		static kl::matrix4x4<T> rotation(const kl::vector3<T>& val)
		{
			const T xRad = kl::to::radians(val.x);
			const T xSin = std::sin(xRad);
			const T xCos = std::cos(xRad);
			kl::matrix4x4<T> xRot;
			xRot[5] = xCos;
			xRot[6] = -xSin;
			xRot[9] = xSin;
			xRot[10] = xCos;

			const T yRad = kl::to::radians(val.y);
			const T ySin = std::sin(yRad);
			const T yCos = std::cos(yRad);
			kl::matrix4x4<T> yRot;
			yRot[0] = yCos;
			yRot[2] = ySin;
			yRot[8] = -ySin;
			yRot[10] = yCos;

			const T zRad = kl::to::radians(val.z);
			const T zSin = std::sin(zRad);
			const T zCos = std::cos(zRad);
			kl::matrix4x4<T> zRot;
			zRot[0] = zCos;
			zRot[1] = -zSin;
			zRot[4] = zSin;
			zRot[5] = zCos;

			return zRot * yRot * xRot;
		}

		// Scaling matrix
		static kl::matrix4x4<T> scaling(const kl::vector3<T>& size)
		{
			kl::matrix4x4<T> temp;
			temp[0] = size.x;
			temp[5] = size.y;
			temp[10] = size.z;
			return temp;
		}

		// Perspective projection matrix
		static kl::matrix4x4<T> perspective(T fov, T ar, T zNear, T zFar)
		{
			const T tanHalf = T(1.0f / tan(kl::to::radians(fov) * 0.5f));
			kl::matrix4x4<T> temp;
			temp[0] = tanHalf / ar;
			temp[5] = tanHalf;
			temp[10] = (-zFar - zNear) / (zNear - zFar);
			temp[11] = T(2.0f * zNear * zFar) / (zNear - zFar);
			temp[14] = T(1.0f);
			temp[15] = T(0.0f);
			return temp;
		}

		// Orthographics projection matrix
		static kl::matrix4x4<T> orthographic(T left, T right, T bottom, T top, T nearZ, T farZ)
		{
			kl::matrix4x4<T> temp;
			temp[0] = T(2.0f) / (right - left);
			temp[5] = T(2.0f) / (top - bottom);
			temp[10] = T(-2.0f) / (farZ - nearZ);
			temp[3] = -(right + left) / (right - left);
			temp[7] = -(top + bottom) / (top - bottom);
			temp[11] = -(farZ + nearZ) / (farZ - nearZ);
			return temp;
		}

		// "Look at" matrix
		static kl::matrix4x4<T> lookAt(const kl::vector3<T>& position, const kl::vector3<T>& target, const kl::vector3<T>& up)
		{
			const kl::vector3<T> f = (target - position).normalize();
			const kl::vector3<T> s = up.cross(f).normalize();
			const kl::vector3<T> u = f.cross(s);
			kl::matrix4x4<T> temp;
			temp[0] = s.x;
			temp[1] = s.y;
			temp[2] = s.z;
			temp[3] = -s.dot(position);
			temp[4] = u.x;
			temp[5] = u.y;
			temp[6] = u.z;
			temp[7] = -u.dot(position);
			temp[8] = f.x;
			temp[9] = f.y;
			temp[10] = f.z;
			temp[11] = -f.dot(position);
			return temp;
		}
	};
}

namespace kl
{
	using float4x4 = kl::matrix4x4<float>;
	using double4x4 = kl::matrix4x4<double>;
	using int4x4 = kl::matrix4x4<int>;
	using uint4x4 = kl::matrix4x4<unsigned int>;
	using bool4x4 = kl::matrix4x4<bool>;

	using mat4 = float4x4;
	using dmat4 = double4x4;
	using imat4 = int4x4;
	using umat4 = uint4x4;
	using bmat4 = bool4x4;
}
