//
// This software is released under the MIT license.
//
// Copyright (c) 2022 Jino Park
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
#include "impl/common.h"
#include "impl/matrix_type_traits.h"

// Dependencies headers

namespace Peanut::Impl {

    /**
     * @brief Expression class which represents a submatrix by excluding
     *        given row and column.
     * @tparam row_ex Row index which will be excluded.
     * @tparam col_ex Column index which will be excluded.
     * @tparam E Matrix expression type.
     */
    template<Index row_ex, Index col_ex, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_ex, E::row> && is_between_v<0, col_ex, E::col>
    struct MatrixSub : public MatrixExpr<MatrixSub<row_ex, col_ex, E>> {
        using Type = typename E::Type;
        MatrixSub(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const {
            return x.elem(r < row_ex ? r : r + 1, c < col_ex ? c : c + 1);
        }

        static constexpr Index row = E::row - 1;
        static constexpr Index col = E::col - 1;

        inline Matrix<Type, row, col> eval() const {
            return Matrix<Type, row, col>(*this);
        }

        const E &x;
    };
}

namespace Peanut {
    /**
     * @brief Get a submatrix matrix by excluding given row and column.
     *        See `Impl::MatrixSub`
     * @tparam row_ex Row index which will be excluded.
     * @tparam col_ex Column index which will be excluded.
     * @tparam E Matrix expression type.
     * @return Constructed `Impl::MatrixSub` instance
     *
     *     Matrix<int, 4, 4> mat{1,2,3,4,
     *                           5,6,7,8,
     *                           9,0,1,2,
     *                           3,4,5,6};
     *
     *     Matrix<int, 3, 3> ev = SubMat<1,2>(mat11);
     *     // 1 2 4
     *     // 9 0 2
     *     // 3 4 6
     *
     */
    template<Index row_ex, Index col_ex, typename E>
        requires is_matrix_v<E> && is_between_v<0, row_ex, E::row> && is_between_v<0, col_ex, E::col>
    Impl::MatrixSub<row_ex, col_ex, E> SubMat(const MatrixExpr<E> &x) {
        return Impl::MatrixSub<row_ex, col_ex, E>(static_cast<const E &>(x));
    }
}