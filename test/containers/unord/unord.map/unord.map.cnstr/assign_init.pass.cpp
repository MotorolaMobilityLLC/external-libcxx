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
// class unordered_map

// unordered_map& operator=(initializer_list<value_type> il);

#include <unordered_map>
#include <string>
#include <cassert>

#include "../../../test_compare.h"
#include "../../../test_hash.h"

int main()
{
#ifdef _LIBCPP_MOVE
    {
        typedef std::allocator<std::pair<const int, std::string> > A;
        typedef std::unordered_map<int, std::string,
                                   test_hash<std::hash<int> >,
                                   test_compare<std::equal_to<int> >,
                                   A
                                   > C;
        typedef std::pair<int, std::string> P;
        C c =   {
                    P(4, "four"),
                    P(1, "four"),
                    P(2, "four"),
                };
        c =     {
                    P(1, "one"),
                    P(2, "two"),
                    P(3, "three"),
                    P(4, "four"),
                    P(1, "four"),
                    P(2, "four"),
                };
        assert(c.bucket_count() >= 5);
        assert(c.size() == 4);
        assert(c.at(1) == "one");
        assert(c.at(2) == "two");
        assert(c.at(3) == "three");
        assert(c.at(4) == "four");
        assert(std::distance(c.begin(), c.end()) == c.size());
        assert(std::distance(c.cbegin(), c.cend()) == c.size());
        assert(c.load_factor() == (float)c.size()/c.bucket_count());
        assert(c.max_load_factor() == 1);
    }
#endif
}
