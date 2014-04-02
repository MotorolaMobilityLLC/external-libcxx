//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <future>

// class future<R>

// ~future();

#include <future>
#include <cassert>

#include "../test_allocator.h"

int main()
{
    assert(test_alloc_base::count == 0);
    {
        typedef int T;
        std::future<T> f;
        {
            std::promise<T> p(std::allocator_arg, test_allocator<T>());
            assert(test_alloc_base::count == 1);
            f = p.get_future();
            assert(test_alloc_base::count == 1);
            assert(f.valid());
        }
        assert(test_alloc_base::count == 1);
        assert(f.valid());
    }
    assert(test_alloc_base::count == 0);
    {
        typedef int& T;
        std::future<T> f;
        {
            std::promise<T> p(std::allocator_arg, test_allocator<int>());
            assert(test_alloc_base::count == 1);
            f = p.get_future();
            assert(test_alloc_base::count == 1);
            assert(f.valid());
        }
        assert(test_alloc_base::count == 1);
        assert(f.valid());
    }
    assert(test_alloc_base::count == 0);
    {
        typedef void T;
        std::future<T> f;
        {
            std::promise<T> p(std::allocator_arg, test_allocator<T>());
            assert(test_alloc_base::count == 1);
            f = p.get_future();
            assert(test_alloc_base::count == 1);
            assert(f.valid());
        }
        assert(test_alloc_base::count == 1);
        assert(f.valid());
    }
    assert(test_alloc_base::count == 0);
}
