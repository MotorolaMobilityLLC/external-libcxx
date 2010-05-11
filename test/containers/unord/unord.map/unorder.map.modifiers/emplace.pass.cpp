//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// template <class... Args>
//     pair<iterator, bool> emplace(Args&&... args);

#include <unordered_map>
#include <cassert>

#include "../../../Emplaceable.h"

int main()
{
#ifdef _LIBCPP_MOVE
    {
        typedef std::unordered_map<int, Emplaceable> C;
        typedef std::pair<C::iterator, bool> R;
        C c;
        R r = c.emplace(3);
        assert(r.second);
        assert(c.size() == 1);
        assert(r.first->first == 3);
        assert(r.first->second == Emplaceable());

        r = c.emplace(std::pair<const int, Emplaceable>(4, Emplaceable(5, 6)));
        assert(r.second);
        assert(c.size() == 2);
        assert(r.first->first == 4);
        assert(r.first->second == Emplaceable(5, 6));

        r = c.emplace(5, 6, 7);
        assert(r.second);
        assert(c.size() == 3);
        assert(r.first->first == 5);
        assert(r.first->second == Emplaceable(6, 7));
    }
#endif
}
