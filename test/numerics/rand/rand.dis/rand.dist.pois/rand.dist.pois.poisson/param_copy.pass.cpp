//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class poisson_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

int main()
{
    {
        typedef std::poisson_distribution<> D;
        typedef D::param_type param_type;
        param_type p0(.125);
        param_type p = p0;
        assert(p.mean() == .125);
    }
}
