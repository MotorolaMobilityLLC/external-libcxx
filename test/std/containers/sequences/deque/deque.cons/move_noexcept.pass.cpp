//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <deque>

// deque(deque&&)
//        noexcept(is_nothrow_move_constructible<allocator_type>::value);

// This tests a conforming extension

// UNSUPPORTED: c++98, c++03

#include <deque>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

template <class T>
struct some_alloc
{
    typedef T value_type;
    some_alloc(const some_alloc&);
};

int main()
{
    {
        typedef std::deque<MoveOnly> C;
        LIBCPP_STATIC_ASSERT(std::is_nothrow_move_constructible<C>::value, "");
    }
    {
        typedef std::deque<MoveOnly, test_allocator<MoveOnly>> C;
        LIBCPP_STATIC_ASSERT(std::is_nothrow_move_constructible<C>::value, "");
    }
    {
        typedef std::deque<MoveOnly, other_allocator<MoveOnly>> C;
        LIBCPP_STATIC_ASSERT(std::is_nothrow_move_constructible<C>::value, "");
    }
    {
        typedef std::deque<MoveOnly, some_alloc<MoveOnly>> C;
        static_assert(!std::is_nothrow_move_constructible<C>::value, "");
    }
}
