//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <array>

// template <class T, size_t N >
// struct array

// Test the size and alignment matches that of an array of a given type.

#include <array>
#include <iterator>
#include <type_traits>
#include <cstddef>

#include "test_macros.h"

template <class T, size_t Size>
void test() {
  typedef T CArrayT[Size == 0 ? 1 : Size];
  typedef std::array<T, Size> ArrayT;
  static_assert(sizeof(CArrayT) == sizeof(ArrayT), "");
  static_assert(TEST_ALIGNOF(CArrayT) == TEST_ALIGNOF(ArrayT), "");
}

template <class T>
void test_type() {
  test<T, 1>();
  test<T, 42>();
  test<T, 0>();
}

struct TEST_ALIGNAS(TEST_ALIGNOF(std::max_align_t) * 2) TestType1 {

};

struct TEST_ALIGNAS(TEST_ALIGNOF(std::max_align_t) * 2) TestType2 {
  char data[1000];
};

int main() {
  test_type<char>();
  test_type<int>();
  test_type<double>();
  test_type<long double>();
  test_type<std::max_align_t>();
  test_type<TestType1>();
  test_type<TestType2>();
}
