//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <list>

// void push_front(value_type&& x);

#include <list>
#include <cassert>

#include "../../../MoveOnly.h"

int main()
{
#ifdef _LIBCPP_MOVE
    std::list<MoveOnly> l1;
    l1.push_front(MoveOnly(1));
    assert(l1.size() == 1);
    assert(l1.front() == MoveOnly(1));
    l1.push_front(MoveOnly(2));
    assert(l1.size() == 2);
    assert(l1.front() == MoveOnly(2));
    assert(l1.back() == MoveOnly(1));
#endif  // _LIBCPP_MOVE
}
