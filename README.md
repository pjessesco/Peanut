# Peanut

[![Unit Test (macOS, AppleClang)](https://github.com/pjessesco/peanut/actions/workflows/unittest_macos_appleclang.yml/badge.svg?branch=main)](https://github.com/pjessesco/peanut/actions/workflows/unittest_macos_appleclang.yml) 
[![Unit Test (macOS, Clang)](https://github.com/pjessesco/peanut/actions/workflows/unittest_macos_clang.yml/badge.svg?branch=main)](https://github.com/pjessesco/peanut/actions/workflows/unittest_macos_clang.yml) 
[![Unit Test (Windows MSVC)](https://github.com/pjessesco/peanut/actions/workflows/unittest_windows_msvc.yml/badge.svg?branch=main)](https://github.com/pjessesco/peanut/actions/workflows/unittest_windows_msvc.yml)
[![Unit Test (Windows Clang)](https://github.com/pjessesco/peanut/actions/workflows/unittest_windows_clang.yml/badge.svg?branch=main)](https://github.com/pjessesco/peanut/actions/workflows/unittest_windows_clang.yml) 
[![Deploy doxygen documents](https://github.com/pjessesco/peanut/actions/workflows/docs.yml/badge.svg?branch=main)](https://github.com/pjessesco/peanut/actions/workflows/docs.yml)

**Peanut** is a header-only matrix library using C++20 without any external dependencies, (except unit test), following a `expression templates` concept. It first constructs matrix expression as a user provides, and evaluates it using an automatically generated code in a compile-time.

```
Matrix<int, 4, 4> mat1{1,2,3,4,
                       1,2,3,4,
                       1,2,3,4,
                       1,2,3,4};

Matrix<int, 3, 5> mat2{1,2,3,4,5,
                       1,2,3,4,5,
                       1,2,3,4,5};

// Peanut first build expression tree,
// `result` type is `MatrixMult<MatrixSubtract<MatrixSum<Matrix<int, 4, 4>, MatrixInverse<Matrix<int, 4, 4>>>, MatrixMinor<Matrix<int, 4, 4>>>, MatrixSub<2, 1, MatrixMult<MatrixTranspose<Matrix<int, 3, 5>>, Matrix<int, 3, 5>>>>`.
auto result = (mat1 + Inverse(mat1) - Minor(mat1)) * SubMat<2, 1>(T(mat2) * mat2);

// then evaluate it when `eval()` is called or assigned to `Matrix` type variable.
/* Matrix<int, 4, 4> */ auto e1 = result.eval();
Matrix<int, 4, 4> e2 = result;
```

### Features
- Arbitrary size matrix expression
- Lazy evaluation
- Unit test

### Usage
Copy `Peanut` directory to your project, and add and its path to the include path. If you're using CMake, add :

    add_subdirectory(ext/peanut)
    include_directories(ext/peanut/include)

Then include as below :

    #include <Peanut/Peanut.h>

Refer [documentation](https://pjessesco.github.io/peanut/) for more information.
