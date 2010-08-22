//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <forward_list>

// iterator insert_after(const_iterator p, size_type n, const value_type& v);

#include <forward_list>
#include <cassert>

int main()
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        typedef C::iterator I;
        C c;
        I i = c.insert_after(c.cbefore_begin(), 0, 0);
        assert(i == c.before_begin());
        assert(distance(c.begin(), c.end()) == 0);

        i = c.insert_after(c.cbefore_begin(), 3, 3);
        assert(i == next(c.before_begin(), 3));
        assert(distance(c.begin(), c.end()) == 3);
        assert(*next(c.begin(), 0) == 3);
        assert(*next(c.begin(), 1) == 3);
        assert(*next(c.begin(), 2) == 3);

        i = c.insert_after(c.begin(), 2, 2);
        assert(i == next(c.begin(), 2));
        assert(distance(c.begin(), c.end()) == 5);
        assert(*next(c.begin(), 0) == 3);
        assert(*next(c.begin(), 1) == 2);
        assert(*next(c.begin(), 2) == 2);
        assert(*next(c.begin(), 3) == 3);
        assert(*next(c.begin(), 4) == 3);
    }
}
