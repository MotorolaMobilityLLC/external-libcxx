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
// class chi_squared_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

int main()
{
    {
        typedef std::chi_squared_distribution<> D;
        typedef D::param_type param_type;
        param_type p;
        assert(p.n() == 1);
    }
    {
        typedef std::chi_squared_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10);
        assert(p.n() == 10);
    }
}
