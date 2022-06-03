#pragma once

#include "math/matrix.h"
#include "math/vector3.h"


namespace kl {
	template<typename T> struct matrix3x3 : public kl::matrix<T, 3, 3> {

		matrix3x3() {}
		matrix3x3(const kl::vector3<T>& r0, const kl::vector3<T>& r1, const kl::vector3<T>& r2) {
			(*this)[0] = r0.x; (*this)[1] = r0.y; (*this)[2] = r0.z;
			(*this)[3] = r1.x; (*this)[4] = r1.y; (*this)[5] = r1.z;
			(*this)[6] = r2.x; (*this)[7] = r2.y; (*this)[8] = r2.z;
		}
		matrix3x3(const kl::matrix<T, 3, 3>& obj) : kl::matrix<T, 3, 3>(obj) {}

		// Inverse matrix
		void inv(kl::matrix3x3<T>& out) const {
			out[0] = (*this)[4] * (*this)[8] - (*this)[7] * (*this)[5];
			out[1] = (*this)[2] * (*this)[7] - (*this)[1] * (*this)[8];
			out[2] = (*this)[1] * (*this)[5] - (*this)[2] * (*this)[4];
			out[3] = (*this)[5] * (*this)[6] - (*this)[3] * (*this)[8];
			out[4] = (*this)[0] * (*this)[8] - (*this)[2] * (*this)[6];
			out[5] = (*this)[3] * (*this)[2] - (*this)[0] * (*this)[5];
			out[6] = (*this)[3] * (*this)[7] - (*this)[6] * (*this)[4];
			out[7] = (*this)[6] * (*this)[1] - (*this)[0] * (*this)[7];
			out[8] = (*this)[0] * (*this)[4] - (*this)[3] * (*this)[1];
			out /= (
				(*this)[0] * ((*this)[4] * (*this)[8] - (*this)[7] * (*this)[5]) -
				(*this)[1] * ((*this)[3] * (*this)[8] - (*this)[5] * (*this)[6]) +
				(*this)[2] * ((*this)[3] * (*this)[7] - (*this)[4] * (*this)[6])
				);
		}
		kl::matrix3x3<T> inv() const {
			kl::matrix3x3<T> temp;
			inv(temp);
			return temp;
		}

		// Translation matrix
		static kl::matrix3x3<T> translation(const kl::vector2<T>& val) {
			kl::matrix3x3<T> temp;
			temp[2] = val.x;
			temp[5] = val.y;
			return temp;
		}

		// Rotation matrix
		static kl::matrix3x3<T> rotation(const T& val) {
			const T zSin = std::sin(kl::math::toRads(val));
			const T zCos = std::cos(kl::math::toRads(val));
			kl::matrix3x3<T> temp;
			temp[0] = zCos;
			temp[1] = -zSin;
			temp[3] = zSin;
			temp[4] = zCos;
			return temp;
		}

		// Scaling matrix
		static kl::matrix3x3<T> scaling(const kl::vector2<T>& val) {
			kl::matrix3x3<T> temp;
			temp[0] = val.x;
			temp[4] = val.y;
			return temp;
		}

	};
}

namespace kl {
	using float3x3 = kl::matrix3x3<float>;
	using double3x3 = kl::matrix3x3<double>;
	using int3x3 = kl::matrix3x3<int>;
	using uint3x3 = kl::matrix3x3<unsigned int>;
	using bool3x3 = kl::matrix3x3<bool>;

	using mat3 = float3x3;
	using dmat3 = double3x3;
	using imat3 = int3x3;
	using umat3 = uint3x3;
	using bmat3 = bool3x3;
}
