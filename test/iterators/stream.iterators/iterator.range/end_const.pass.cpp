//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <iterator>

// template <class C> auto end(const C& c) -> decltype(c.end());

#include <vector>
#include <cassert>

int main()
{
    int ia[] = {1, 2, 3};
    const std::vector<int> v(ia, ia + sizeof(ia)/sizeof(ia[0]));
    std::vector<int>::const_iterator i = end(v);
    assert(i == v.cend());
}
