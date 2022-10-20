#pragma once

#include "math/matrix.h"


namespace kl {
    template <typename T>
    struct matrix2_x2 : matrix<T, 2, 2>
    {
        matrix2_x2()
        {}

        matrix2_x2(const vector2<T>& r0, const vector2<T>& r1)
        {
            (*this)[0] = r0.x;
            (*this)[1] = r0.y;
            (*this)[2] = r1.x;
            (*this)[3] = r1.y;
        }

        matrix2_x2(const matrix<T, 2, 2>& obj)
            : matrix<T, 2, 2>(obj)
        {}
    };
}

namespace kl {
    using float2x2 = matrix2_x2<float>;
    using double2x2 = matrix2_x2<double>;
    using int2x2 = matrix2_x2<int32_t>;
    using uint2x2 = matrix2_x2<uint32_t>;
    using bool2x2 = matrix2_x2<bool>;

    using mat2 = float2x2;
    using d_mat2 = double2x2;
    using i_mat2 = int2x2;
    using u_mat2 = uint2x2;
    using b_mat2 = bool2x2;
}
