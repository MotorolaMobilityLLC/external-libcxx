
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// <variant> feature macros

/*  Constant                                    Value
    __cpp_lib_variant                           201606L

*/

#include <variant>
#include "test_macros.h"

int main()
{
//  ensure that the macros that are supposed to be defined in <variant> are defined.

/*
#if !defined(__cpp_lib_fooby)
# error "__cpp_lib_fooby is not defined"
#elif __cpp_lib_fooby < 201606L
# error "__cpp_lib_fooby has an invalid value"
#endif
*/
}
