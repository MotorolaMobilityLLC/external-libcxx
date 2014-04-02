//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class negative_binomial_distribution

// explicit negative_binomial_distribution(const param_type& parm);

#include <random>
#include <cassert>

int main()
{
    {
        typedef std::negative_binomial_distribution<> D;
        typedef D::param_type P;
        P p(5, 0.25);
        D d(p);
        assert(d.k() == 5);
        assert(d.p() == 0.25);
    }
}
