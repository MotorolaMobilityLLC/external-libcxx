//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// char narrow(char_type c, char dfault) const;

#include <ios>
#include <cassert>

int main()
{
    const std::ios ios(0);
    assert(ios.narrow('c', '*') == 'c');
    assert(ios.narrow('\xFE', '*') == '*');
}
