//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// template<class D, class T> D* get_deleter(const shared_ptr<T>& p);

#include <memory>
#include <cassert>
#include "../test_deleter.h"

struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int main()
{
    {
        {
            A* ptr = new A;
            std::shared_ptr<A> p(ptr, test_deleter<A>(3));
            test_deleter<A>* d = std::get_deleter<test_deleter<A> >(p);
            assert(test_deleter<A>::count == 1);
            assert(test_deleter<A>::dealloc_count == 0);
            assert(d);
            assert(d->state() == 3);
        }
        assert(A::count == 0);
        assert(test_deleter<A>::count == 0);
        assert(test_deleter<A>::dealloc_count == 1);
    }
    test_deleter<A>::dealloc_count = 0;
    {
        {
            std::shared_ptr<A> p(nullptr, test_deleter<A>(3));
            test_deleter<A>* d = std::get_deleter<test_deleter<A> >(p);
            assert(test_deleter<A>::count == 1);
            assert(test_deleter<A>::dealloc_count == 0);
            assert(d);
            assert(d->state() == 3);
        }
        assert(A::count == 0);
        assert(test_deleter<A>::count == 0);
        assert(test_deleter<A>::dealloc_count == 1);
    }
    test_deleter<A>::dealloc_count = 0;
    {
        std::shared_ptr<A> p(nullptr, test_deleter<A>(3));
        std::default_delete<A>* d = std::get_deleter<std::default_delete<A> >(p);
        assert(d == 0);
    }
}
