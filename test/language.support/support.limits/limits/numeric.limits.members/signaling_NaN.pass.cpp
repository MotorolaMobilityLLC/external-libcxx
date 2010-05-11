//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// test numeric_limits

// signaling_NaN()

#include <limits>
#include <cmath>
#include <type_traits>
#include <cassert>

template <class T>
void
test_imp(std::true_type)
{
    assert(std::isnan(std::numeric_limits<T>::signaling_NaN()));
    assert(std::isnan(std::numeric_limits<const T>::signaling_NaN()));
    assert(std::isnan(std::numeric_limits<volatile T>::signaling_NaN()));
    assert(std::isnan(std::numeric_limits<const volatile T>::signaling_NaN()));
}

template <class T>
void
test_imp(std::false_type)
{
    assert(std::numeric_limits<T>::signaling_NaN() == T());
    assert(std::numeric_limits<const T>::signaling_NaN() == T());
    assert(std::numeric_limits<volatile T>::signaling_NaN() == T());
    assert(std::numeric_limits<const volatile T>::signaling_NaN() == T());
}

template <class T>
inline
void
test()
{
    test_imp<T>(std::is_floating_point<T>());
}

int main()
{
    test<bool>();
    test<char>();
    test<signed char>();
    test<unsigned char>();
    test<wchar_t>();
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
    test<char16_t>();
    test<char32_t>();
#endif
    test<short>();
    test<unsigned short>();
    test<int>();
    test<unsigned int>();
    test<long>();
    test<unsigned long>();
    test<long long>();
    test<unsigned long long>();
    test<float>();
    test<double>();
    test<long double>();
}
