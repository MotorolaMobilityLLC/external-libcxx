//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <random>

// class seed_seq;

// template<class T>
//     seed_seq(initializer_list<T> il);

#include <random>
#include <cassert>

int main()
{
#ifdef _LIBCPP_MOVE
    std::seed_seq s= {5, 4, 3, 2, 1};
    assert(s.size() == 5);
    unsigned b[5] = {0};
    s.param(b);
    assert(b[0] == 5);
    assert(b[1] == 4);
    assert(b[2] == 3);
    assert(b[3] == 2);
    assert(b[4] == 1);
#endif
}
