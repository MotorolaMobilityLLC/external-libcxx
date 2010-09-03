//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <future>

// class shared_future<R>

// shared_future(shared_future&& rhs);

#include <future>
#include <cassert>

int main()
{
    {
        typedef int T;
        std::promise<T> p;
        std::shared_future<T> f0 = p.get_future();
        std::shared_future<T> f = std::move(f0);
        assert(!f0.valid());
        assert(f.valid());
    }
    {
        typedef int T;
        std::shared_future<T> f0;
        std::shared_future<T> f = std::move(f0);
        assert(!f0.valid());
        assert(!f.valid());
    }
    {
        typedef int& T;
        std::promise<T> p;
        std::shared_future<T> f0 = p.get_future();
        std::shared_future<T> f = std::move(f0);
        assert(!f0.valid());
        assert(f.valid());
    }
    {
        typedef int& T;
        std::shared_future<T> f0;
        std::shared_future<T> f = std::move(f0);
        assert(!f0.valid());
        assert(!f.valid());
    }
    {
        typedef void T;
        std::promise<T> p;
        std::shared_future<T> f0 = p.get_future();
        std::shared_future<T> f = std::move(f0);
        assert(!f0.valid());
        assert(f.valid());
    }
    {
        typedef void T;
        std::shared_future<T> f0;
        std::shared_future<T> f = std::move(f0);
        assert(!f0.valid());
        assert(!f.valid());
    }
}
