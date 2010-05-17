//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class cauchy_distribution

// cauchy_distribution(const cauchy_distribution&);

#include <random>
#include <cassert>

void
test1()
{
    typedef std::cauchy_distribution<> D;
    D d1(.5, 1.75);
    D d2 = d1;
    assert(d1 == d2);
}

int main()
{
    test1();
}
