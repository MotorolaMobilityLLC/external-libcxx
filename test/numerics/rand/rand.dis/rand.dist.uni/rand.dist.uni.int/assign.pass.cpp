//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <random>

// template<class _IntType = int>
// class uniform_int_distribution

// uniform_int_distribution& operator=(const uniform_int_distribution&);

#include <random>
#include <cassert>

void
test1()
{
    typedef std::uniform_int_distribution<long> D;
    D d1(2, 5);
    D d2;
    assert(d1 != d2);
    d2 = d1;
    assert(d1 == d2);
}

int main()
{
    test1();
}
