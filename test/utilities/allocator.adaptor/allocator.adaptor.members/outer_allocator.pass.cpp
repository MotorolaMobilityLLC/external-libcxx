//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <memory>

// template <class OuterAlloc, class... InnerAllocs>
//   class scoped_allocator_adaptor

// outer_allocator_type& outer_allocator();
// const outer_allocator_type& outer_allocator() const;

#include <scoped_allocator>
#include <cassert>

#include "../allocators.h"

int main()
{
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    {
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        A a(A1<int>(5));
        assert(a.outer_allocator() == A1<int>(5));
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
        A a(A1<int>(5), A2<int>(6));
        assert(a.outer_allocator() == A1<int>(5));
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A a(A1<int>(5), A2<int>(6), A3<int>(8));
        assert(a.outer_allocator() == A1<int>(5));
    }

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
}
