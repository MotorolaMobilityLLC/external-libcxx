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

// template <class Y, class D> shared_ptr& operator=(unique_ptr<Y, D>&& r);

#include <memory>
#include <type_traits>
#include <cassert>

struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};

int B::count = 0;

struct A
    : public B
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
        std::unique_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = std::move(pA);
            assert(B::count == 1);
            assert(A::count == 1);
            assert(pB.use_count() == 1);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::unique_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = std::move(pA);
            assert(B::count == 0);
            assert(A::count == 0);
            assert(pB.use_count() == 1);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::unique_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = std::move(pA);
            assert(B::count == 1);
            assert(A::count == 1);
            assert(pB.use_count() == 1);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::unique_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = std::move(pA);
            assert(B::count == 0);
            assert(A::count == 0);
            assert(pB.use_count() == 1);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
}
