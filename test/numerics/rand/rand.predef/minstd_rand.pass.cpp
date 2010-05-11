//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <random>

// typedef linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647>
//                                                                 minstd_rand;

#include <random>
#include <cassert>

int main()
{
    std::minstd_rand e;
    e.discard(9999);
    assert(e() == 399268537u);
}
