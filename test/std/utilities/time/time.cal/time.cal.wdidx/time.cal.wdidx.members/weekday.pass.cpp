//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class weekday_indexed;

// constexpr chrono::weekday weekday() const noexcept;
//  Returns: wd_

#include <chrono>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

int main()
{
    using weekday         = std::chrono::weekday;
    using weekday_indexed = std::chrono::weekday_indexed;

    ASSERT_NOEXCEPT(                                std::declval<const weekday_indexed>().weekday());
    ASSERT_SAME_TYPE(std::chrono::weekday, decltype(std::declval<const weekday_indexed>().weekday()));

    static_assert( weekday_indexed{}.weekday() == weekday{},                                   "");
    static_assert( weekday_indexed{std::chrono::Tuesday, 0}.weekday() == std::chrono::Tuesday, "");

    for (unsigned i = 0; i <= 6; ++i)
    {
        weekday_indexed wdi(weekday{i}, 2);
        assert( static_cast<unsigned>(wdi.weekday()) == i);
    }
}
