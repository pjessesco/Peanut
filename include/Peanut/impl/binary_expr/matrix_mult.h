//
// This software is released under the MIT license.
//
// Copyright (c) 2022-2024 Jino Park
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#pragma once

// Standard headers

// Peanut headers
#include <Peanut/impl/common.h>
#include <Peanut/impl/matrix_type_traits.h>

// Dependencies headers

namespace Peanut::Impl {

    /**
     * @brief Expression class which represents `operator*()`.
     * @details Note that `MatrixMult` evaluates its operands internally
     *          for the performance issues.
     * @tparam E1 Left hand side matrix expression type.
     * @tparam E2 Right hand side matrix expression type.
     */
    template<typename E1, typename E2>
        requires(E1::Col == E2::Row)
    struct MatrixMult : public MatrixExpr<MatrixMult<E1, E2>> {
        using Type = typename E1::Type;
        MatrixMult(const E1 &_x, const E2 &_y) {
            const Matrix<Type, E1::Row, E1::Col> x_eval = _x;
            const Matrix<Type, E2::Row, E2::Col> y_eval = _y;
            evalVal = Matrix<Type, E1::Row, E2::Col>::zeros();

            for (int i = 0; i < E1::Row; i++) {
                for (int j = 0; j < E2::Col; j++) {
                    for (int k = 0; k < E1::Col; k++) {
                        evalVal.elem(i, j) += x_eval.elem(i, k) * y_eval.elem(k, j);
                    }
                }
            }
        }

        // Static polymorphism implementation of MatrixExpr
        INLINE auto elem(Index r, Index c) const {
            return evalVal.elem(r, c);
        }

        static constexpr Index Row = E1::Row;
        static constexpr Index Col = E2::Col;

        INLINE auto eval() const {
            return Matrix<Type, Row, Col>(*this);
        }

        Matrix<Type, E1::Row, E2::Col> evalVal;
    };

}

namespace Peanut {

    /**
     * @brief Multiplication between matrices. See `Impl::MatrixMult`.
     * @tparam E1 Left hand side matrix expression type.
     * @tparam E2 Right hand side matrix expression type.
     * @return Constructed `Impl::MatrixMult` instance
     */
    template<typename E1, typename E2>
        requires(E1::Col == E2::Row)
    Impl::MatrixMult<E1, E2> operator*(const MatrixExpr<E1> &x, const MatrixExpr<E2> &y) {
        return Impl::MatrixMult<E1, E2>(static_cast<const E1 &>(x), static_cast<const E2 &>(y));
    }
}
