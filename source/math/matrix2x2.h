#pragma once

#include "math/matrix.h"
#include "math/vector2.h"


namespace kl {
	template<typename T> struct matrix2x2 : public kl::matrix<T, 2, 2> {
		matrix2x2() {}
		matrix2x2(const kl::vector2<T>& r0, const kl::vector2<T>& r1) {
			(*this)[0] = r0.x; (*this)[1] = r0.y;
			(*this)[2] = r1.x; (*this)[3] = r1.y;
		}
		matrix2x2(const kl::matrix<T, 2, 2>& obj) : kl::matrix<T, 2, 2>(obj) {}
	};
}

namespace kl {
	using float2x2 = kl::matrix2x2<float>;
	using double2x2 = kl::matrix2x2<double>;
	using int2x2 = kl::matrix2x2<int>;
	using uint2x2 = kl::matrix2x2<unsigned int>;
	using bool2x2 = kl::matrix2x2<bool>;

	using mat2 = float2x2;
	using dmat2 = double2x2;
	using imat2 = int2x2;
	using umat2 = uint2x2;
	using bmat2 = bool2x2;
}
