//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// unordered_set(initializer_list<value_type> il, size_type n,
//               const hasher& hf, const key_equal& eql);

#include <unordered_set>
#include <cassert>
#include <cfloat>
#include <cmath>

#include "../../../test_compare.h"
#include "../../../test_hash.h"
#include "test_allocator.h"
#include "min_allocator.h"

int main()
{
#ifndef _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS
    {
        typedef std::unordered_set<int,
                                   test_hash<std::hash<int> >,
                                   test_compare<std::equal_to<int> >,
                                   test_allocator<int>
                                   > C;
        typedef int P;
        C c({
                P(1),
                P(2),
                P(3),
                P(4),
                P(1),
                P(2)
            },
            7,
            test_hash<std::hash<int> >(8),
            test_compare<std::equal_to<int> >(9)
           );
        assert(c.bucket_count() == 7);
        assert(c.size() == 4);
        assert(c.count(1) == 1);
        assert(c.count(2) == 1);
        assert(c.count(3) == 1);
        assert(c.count(4) == 1);
        assert(c.hash_function() == test_hash<std::hash<int> >(8));
        assert(c.key_eq() == test_compare<std::equal_to<int> >(9));
        assert(c.get_allocator() == test_allocator<int>());
        assert(!c.empty());
        assert(std::distance(c.begin(), c.end()) == c.size());
        assert(std::distance(c.cbegin(), c.cend()) == c.size());
        assert(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        assert(c.max_load_factor() == 1);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_set<int,
                                   test_hash<std::hash<int> >,
                                   test_compare<std::equal_to<int> >,
                                   min_allocator<int>
                                   > C;
        typedef int P;
        C c({
                P(1),
                P(2),
                P(3),
                P(4),
                P(1),
                P(2)
            },
            7,
            test_hash<std::hash<int> >(8),
            test_compare<std::equal_to<int> >(9)
           );
        assert(c.bucket_count() == 7);
        assert(c.size() == 4);
        assert(c.count(1) == 1);
        assert(c.count(2) == 1);
        assert(c.count(3) == 1);
        assert(c.count(4) == 1);
        assert(c.hash_function() == test_hash<std::hash<int> >(8));
        assert(c.key_eq() == test_compare<std::equal_to<int> >(9));
        assert(c.get_allocator() == min_allocator<int>());
        assert(!c.empty());
        assert(std::distance(c.begin(), c.end()) == c.size());
        assert(std::distance(c.cbegin(), c.cend()) == c.size());
        assert(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        assert(c.max_load_factor() == 1);
    }
#endif
#endif  // _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS
}
