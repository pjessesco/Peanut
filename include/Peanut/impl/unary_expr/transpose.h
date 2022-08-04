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
#include <Peanut/impl/common.h>
#include <Peanut/impl/matrix_type_traits.h>

// Dependencies headers

namespace Peanut::Impl {

    /**
     * @brief Expression class which represents a transpose matrix.
     * @tparam E Matrix expression type.
     */
    template<typename E>
        requires is_matrix_v<E>
    struct MatrixTranspose : public MatrixExpr<MatrixTranspose<E>> {
        using Type = typename E::Type;
        MatrixTranspose(const E &x) : x{x} {}

        // Static polymorphism implementation of MatrixExpr
        inline auto elem(Index r, Index c) const {
            return x.elem(c, r);
        }

        static constexpr Index Row = E::Col;
        static constexpr Index Col = E::Row;

        inline Matrix<Type, Row, Col> eval() const {
            return Matrix<Type, Row, Col>(*this);
        }

        const E &x;
    };
}

namespace Peanut{
    /**
     * @brief Transpose operation of matrix. See `Impl::MatrixTranspose`
     *        and https://en.wikipedia.org/wiki/Transpose for details.
     * @tparam E Matrix expression type.
     * @return Constructed `Impl::MatrixTranspose` instance
     */
    template<typename E>
        requires is_matrix_v<E>
    Impl::MatrixTranspose<E> T(const MatrixExpr<E> &x) {
        return Impl::MatrixTranspose<E>(static_cast<const E &>(x));
    }

    /**
     * @brief Template specialization of `T()` which represents a
     *        transpose of a transpose of a matrix. It is equivalent to
     *        a input of the given parameter (i.e., T(T(x)) = x).
     * @tparam E Matrix expression type.
     * @param x `MatrixTranspose<E>` type matrix expression.
     * @return Input of the given parameter `x`
     */
    template<typename E>
        requires is_matrix_v<E>
    E T(const Impl::MatrixTranspose<E> &x) {
        return static_cast<const E &>(x.x);
    }

}
