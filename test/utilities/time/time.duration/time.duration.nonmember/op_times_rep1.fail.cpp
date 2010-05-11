//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// template <class Rep1, class Period, class Rep2> 
//   duration<typename common_type<Rep1, Rep2>::type, Period> 
//   operator*(const duration<Rep1, Period>& d, const Rep2& s);

// template <class Rep1, class Period, class Rep2> 
//   duration<typename common_type<Rep1, Rep2>::type, Period> 
//   operator*(const Rep1& s, const duration<Rep2, Period>& d);

#include <chrono>

#include "../../rep.h"

int main()
{
    std::chrono::duration<Rep> d;
    d = d * 5;
}
