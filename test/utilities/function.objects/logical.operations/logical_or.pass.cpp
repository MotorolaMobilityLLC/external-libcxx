//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <functional>

// logical_or

#include <functional>
#include <type_traits>
#include <cassert>

int main()
{
    typedef std::logical_or<int> F;
    const F f = F();
    static_assert((std::is_base_of<std::binary_function<int, int, bool>, F>::value), "");
    assert(f(36, 36));
    assert(f(36, 0));
    assert(f(0, 36));
    assert(!f(0, 0));
#if _LIBCPP_STD_VER > 11
    typedef std::logical_or<> F2;
    const F2 f2 = F2();
    assert( f2(36, 36));
    assert( f2(36, 36L));
    assert( f2(36L, 36));
    assert( f2(36, 0));
    assert( f2(0, 36));
    assert( f2(36, 0L));
    assert( f2(0, 36L));
    assert(!f2(0, 0));
    assert(!f2(0, 0L));
    assert(!f2(0L, 0));
#endif
}
