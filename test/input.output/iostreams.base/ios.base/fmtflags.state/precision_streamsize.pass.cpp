//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// streamsize precision(streamsize prec);

#include <ios>
#include <cassert>

class test
    : public std::ios
{
public:
    test()
    {
        init(0);
    }
};

int main()
{
    test t;
    assert(t.precision() == 6);
    std::streamsize p = t.precision(10);
    assert(p == 6);
    assert(t.precision() == 10);
}
