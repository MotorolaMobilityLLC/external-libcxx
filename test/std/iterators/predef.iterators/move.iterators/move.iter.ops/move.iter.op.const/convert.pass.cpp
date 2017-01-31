//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <iterator>

// move_iterator

// template <class U>
//   requires HasConstructor<Iter, const U&>
//   move_iterator(const move_iterator<U> &u);
//
//  constexpr in C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

template <class It, class U>
void
test(U u)
{
    const std::move_iterator<U> r2(u);
    std::move_iterator<It> r1 = r2;
    assert(r1.base() == u);
}

struct Base {};
struct Derived : Base {};

int main()
{
    Derived d;

    test<input_iterator<Base*> >(input_iterator<Derived*>(&d));
    test<forward_iterator<Base*> >(forward_iterator<Derived*>(&d));
    test<bidirectional_iterator<Base*> >(bidirectional_iterator<Derived*>(&d));
    test<random_access_iterator<const Base*> >(random_access_iterator<Derived*>(&d));
    test<Base*>(&d);

#if TEST_STD_VER > 14
    {
    constexpr const Derived *p = nullptr;
    constexpr std::move_iterator<const Derived *>     it1 = std::make_move_iterator(p);
    constexpr std::move_iterator<const Base *>        it2(it1);
    static_assert(it2.base() == p);
    }
#endif
}
