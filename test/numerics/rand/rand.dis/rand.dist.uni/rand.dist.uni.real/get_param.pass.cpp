//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class uniform_real_distribution

// param_type param() const;

#include <random>
#include <cassert>

int main()
{
    {
        typedef std::uniform_real_distribution<> D;
        typedef D::param_type P;
        P p(3, 8);
        D d(p);
        assert(d.param() == p);
    }
}
