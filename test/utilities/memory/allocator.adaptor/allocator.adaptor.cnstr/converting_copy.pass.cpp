//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <memory>

// template <class OuterAlloc, class... InnerAllocs>
//   class scoped_allocator_adaptor

// template <class OuterA2>
//   scoped_allocator_adaptor(const scoped_allocator_adaptor<OuterA2,
//                                                           InnerAllocs...>& other);

#include <scoped_allocator>
#include <cassert>

#include "../allocators.h"

int main()
{
#ifdef _LIBCPP_MOVE

    {
        typedef std::scoped_allocator_adaptor<A1<double>> B;
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        B a1(A1<int>(3));
        A1<int>::copy_called = false;
        A1<int>::move_called = false;
        A a2 = a1;
        assert(A1<int>::copy_called == true);
        assert(a2 == a1);
    }
    {
        typedef std::scoped_allocator_adaptor<A1<double>, A2<int>> B;
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
        B a1(A1<int>(4), A2<int>(5));
        A1<int>::copy_called = false;
        A1<int>::move_called = false;
        A2<int>::copy_called = false;
        A2<int>::move_called = false;
        A a2 = a1;
        assert(A1<int>::copy_called == true);
        assert(A2<int>::copy_called == true);
        assert(a2 == a1);
    }
    {
        typedef std::scoped_allocator_adaptor<A1<double>, A2<int>, A3<int>> B;
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        B a1(A1<int>(4), A2<int>(5), A3<int>(6));
        A1<int>::copy_called = false;
        A1<int>::move_called = false;
        A2<int>::copy_called = false;
        A2<int>::move_called = false;
        A3<int>::copy_called = false;
        A3<int>::move_called = false;
        A a2 = a1;
        assert(A1<int>::copy_called == true);
        assert(A2<int>::copy_called == true);
        assert(A3<int>::copy_called == true);
        assert(a2 == a1);
    }

#endif  // _LIBCPP_MOVE
}
