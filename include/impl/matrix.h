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
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <string>
#include <type_traits>

// Peanut headers
#include "common.h"
#include "matrix_type_traits.h"
#include "matrix_unary_op.h"

// Dependencies headers

namespace Peanut {

    template <typename E>
    struct MatrixExpr{
        inline auto elem(Index r, Index c) const{
            return static_cast<const E&>(*this).elem(r, c);
        }

        inline auto& elem(Index r, Index c) {
            return static_cast<const E&>(*this).elem(r, c);
        }

        static constexpr Index row = E::row;
        static constexpr Index col = E::col;
    };

    template<typename T, Index Row, Index Col> requires std::is_arithmetic_v<T> && (Row > 0) && (Col > 0)
    struct Matrix : public MatrixExpr<Matrix<T, Row, Col>>{
        using Type = T;
        Matrix() {m_data.fill(t_0);}
        
        template <typename ...TList> requires std::conjunction_v<std::is_same<T, TList>...>
        Matrix(TList ... tlist) : m_data{std::forward<T>(tlist)...} {}
        
        explicit Matrix(const std::array<T, Row * Col> &data) : m_data{data} {}
        explicit Matrix(const std::vector<T> &data) : m_data{data} {}

        template<typename E>
        Matrix(const MatrixExpr<E> &expr) requires is_equal_type_size_v<E, Matrix>{
            for(Index r=0;r<Row;r++){
                for(Index c=0;c<Col;c++){
                    elem(r, c) = expr.elem(r, c);
                }
            }
        }

        static Matrix zeros() {return Matrix();}

        static Matrix identity() requires is_square_v<Matrix> {
            Matrix a;
            a.m_data.fill(t_0);
            for (Index i = 0; i < Row * Col; i += (Row + 1)) {
                a.m_data[i] = t_1;
            }
            return a;
        }

        // Static polymorphism implementation of MatrixExpr
        inline T elem(Index r, Index c) const{
            assert((0<=r) && (r < Row) && (0<=c) && (c < Col));
            return m_data[Col*r+c];
        }

        inline T& elem(Index r, Index c) {
            assert((0<=r) && (r < Row) && (0<=c) && (c < Col));
            return m_data[Col*r+c];
        }

        static constexpr Index row = Row;
        static constexpr Index col = Col;

        inline Matrix<Type, row, col> eval() const{
            return Matrix<Type, row, col>(*this);
        }

        void print_mat() const {
            for(int r=0;r<Row;r++){
                for (int c = 0; c < Col; c++) {
                    std::cout << elem(r, c) << " ";
                }
                std::cout<<"\n";
            }
        }

        void subtract_row(Index r1, Index r2, T scalar){
            for(int i=0;i<Col;i++){
                elem(r1, i) -= scalar * elem(r2, i);
            }
        }

        Matrix<Float, Row, Col> gaussian_elem() const{
            Matrix<Float, Row, Col> ret = cast_to<Float>(*this);
            for(int j=0;j<Row-1;j++){
                const Float denom = static_cast<Float>(ret.elem(j,j));
                if(is_zero(denom)){
                    continue;
                }
                for(int i=j+1;i<Row;i++){
                    const Float ratio = static_cast<Float>(ret.elem(i, j)) / denom;
                    ret.subtract_row(i, j, ratio);
                }
            }
            return ret;
        }

        constexpr T det() const requires is_square_v<Matrix>{
            if constexpr(Col==1){
                return elem(0, 0);
            }
            else if constexpr (Col==2){
                return elem(0, 0) * elem(1, 1) - elem(0, 1) * elem(1, 0);
            }
            auto upper_triangular = gaussian_elem();
            Float det = upper_triangular.elem(0, 0);
            for(int i=1;i<Row;i++){
                det *= upper_triangular.elem(i, i);
            }
            return det;
        }

        constexpr T det2() const requires is_square_v<Matrix>{
            if constexpr(Col==1){
                return elem(0, 0);
            }
            else if constexpr (Col==2){
                return elem(0, 0) * elem(1, 1) - elem(0, 1) * elem(1, 0);
            }
            else{
                T ret = static_cast<T>(0);
                Peanut::for_<Col>([&] (auto c) {
                    ret += (c.value % 2 ? -1 : 1) * elem(0, c.value) * DeleteRC<0, c.value>(*this).eval().det();
                });
                return ret;
            }
        }

    private:
        static constexpr T t_1 = static_cast<T>(1);
        static constexpr T t_0 = static_cast<T>(0);
        std::array<T, Row*Col> m_data;
    };
}