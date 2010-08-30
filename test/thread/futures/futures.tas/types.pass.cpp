//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <future>

// template<class R, class... ArgTypes>
//     class packaged_task<R(ArgTypes...)>
// {
// public:
//     typedef R result_type;

#include <future>
#include <type_traits>

struct A {};

int main()
{
    static_assert((std::is_same<std::packaged_task<A(int, char)>::result_type, A>::value), "");
}
