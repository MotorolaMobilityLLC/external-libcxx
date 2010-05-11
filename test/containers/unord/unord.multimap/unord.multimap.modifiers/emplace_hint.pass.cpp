//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// template <class... Args>
//     iterator emplace_hint(const_iterator p, Args&&... args);

#include <unordered_map>
#include <cassert>

#include "../../../Emplaceable.h"

int main()
{
#ifdef _LIBCPP_MOVE
    {
        typedef std::unordered_multimap<int, Emplaceable> C;
        typedef C::iterator R;
        C c;
        C::const_iterator e = c.end();
        R r = c.emplace_hint(e, 3);
        assert(c.size() == 1);
        assert(r->first == 3);
        assert(r->second == Emplaceable());

        r = c.emplace_hint(e, std::pair<const int, Emplaceable>(3, Emplaceable(5, 6)));
        assert(c.size() == 2);
        assert(r->first == 3);
        assert(r->second == Emplaceable(5, 6));
        assert(r == next(c.begin()));

        r = c.emplace_hint(r, 3, 6, 7);
        assert(c.size() == 3);
        assert(r->first == 3);
        assert(r->second == Emplaceable(6, 7));
        assert(r == next(c.begin()));
        r = c.begin();
        assert(r->first == 3);
        assert(r->second == Emplaceable());
        r = next(r, 2);
        assert(r->first == 3);
        assert(r->second == Emplaceable(5, 6));
    }
#endif
}
