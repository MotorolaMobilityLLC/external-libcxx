//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// Call erase(const_iterator first, const_iterator last); with both iterators from another container

#if _LIBCPP_DEBUG2 >= 1

#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::terminate())

#include <vector>
#include <cassert>
#include <exception>
#include <cstdlib>

void f1()
{
    std::exit(0);
}

int main()
{
    std::set_terminate(f1);
    int a1[] = {1, 2, 3};
    std::vector<int> l1(a1, a1+3);
    std::vector<int> l2(a1, a1+3);
    std::vector<int>::iterator i = l1.erase(l2.cbegin(), l2.cbegin()+1);
    assert(false);
}

#else

int main()
{
}

#endif
