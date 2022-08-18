#pragma once

#include "Math/Matrix.h"
#include "Math/Vector2.h"


namespace kl {
	template<typename T>
	struct Matrix2x2 : public Matrix<T, 2, 2> {
		Matrix2x2() {}

		Matrix2x2(const Vector2<T>& r0, const Vector2<T>& r1) {
			(*this)[0] = r0.x; (*this)[1] = r0.y;
			(*this)[2] = r1.x; (*this)[3] = r1.y;
		}

		Matrix2x2(const Matrix<T, 2, 2>& obj) : Matrix<T, 2, 2>(obj) {}
	};
}

namespace kl {
	using Float2x2 = Matrix2x2<float>;
	using Double2x2 = Matrix2x2<double>;
	using Int2x2 = Matrix2x2<int>;
	using UInt2x2 = Matrix2x2<uint>;
	using Bool2x2 = Matrix2x2<bool>;

	using Mat2 = Float2x2;
	using DMat2 = Double2x2;
	using IMat2 = Int2x2;
	using UMat2 = UInt2x2;
	using BMat2 = Bool2x2;
}
