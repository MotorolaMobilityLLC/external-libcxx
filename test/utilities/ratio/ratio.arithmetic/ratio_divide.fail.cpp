//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// test ratio_divide

#include <ratio>

int main()
{
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef std::ratio<1, 2> R2;
    typedef std::ratio_divide<R1, R2>::type R;
}
