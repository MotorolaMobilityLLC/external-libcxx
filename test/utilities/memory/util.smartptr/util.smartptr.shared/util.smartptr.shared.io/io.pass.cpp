//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// template<class CharT, class Traits, class Y>
//   basic_ostream<CharT, Traits>&
//   operator<<(basic_ostream<CharT, Traits>& os, shared_ptr<Y> const& p);

#include <memory>
#include <sstream>
#include <cassert>

int main()
{
    std::shared_ptr<int> p(new int(3));
    std::ostringstream os;
    assert(os.str().empty());
    os << p;
    assert(!os.str().empty());
}
