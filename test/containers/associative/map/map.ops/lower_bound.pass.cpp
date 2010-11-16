//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <map>

// class map

//       iterator lower_bound(const key_type& k);
// const_iterator lower_bound(const key_type& k) const;

#include <map>
#include <cassert>

int main()
{
    typedef std::pair<const int, double> V;
    typedef std::map<int, double> M;
    {
        typedef M::iterator R;
        V ar[] =
        {
            V(5, 5),
            V(7, 6),
            V(9, 7),
            V(11, 8),
            V(13, 9),
            V(15, 10),
            V(17, 11),
            V(19, 12)
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.lower_bound(5);
        assert(r == m.begin());
        r = m.lower_bound(7);
        assert(r == next(m.begin()));
        r = m.lower_bound(9);
        assert(r == next(m.begin(), 2));
        r = m.lower_bound(11);
        assert(r == next(m.begin(), 3));
        r = m.lower_bound(13);
        assert(r == next(m.begin(), 4));
        r = m.lower_bound(15);
        assert(r == next(m.begin(), 5));
        r = m.lower_bound(17);
        assert(r == next(m.begin(), 6));
        r = m.lower_bound(19);
        assert(r == next(m.begin(), 7));
        r = m.lower_bound(4);
        assert(r == next(m.begin(), 0));
        r = m.lower_bound(6);
        assert(r == next(m.begin(), 1));
        r = m.lower_bound(8);
        assert(r == next(m.begin(), 2));
        r = m.lower_bound(10);
        assert(r == next(m.begin(), 3));
        r = m.lower_bound(12);
        assert(r == next(m.begin(), 4));
        r = m.lower_bound(14);
        assert(r == next(m.begin(), 5));
        r = m.lower_bound(16);
        assert(r == next(m.begin(), 6));
        r = m.lower_bound(18);
        assert(r == next(m.begin(), 7));
        r = m.lower_bound(20);
        assert(r == next(m.begin(), 8));
    }
    {
        typedef M::const_iterator R;
        V ar[] =
        {
            V(5, 5),
            V(7, 6),
            V(9, 7),
            V(11, 8),
            V(13, 9),
            V(15, 10),
            V(17, 11),
            V(19, 12)
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.lower_bound(5);
        assert(r == m.begin());
        r = m.lower_bound(7);
        assert(r == next(m.begin()));
        r = m.lower_bound(9);
        assert(r == next(m.begin(), 2));
        r = m.lower_bound(11);
        assert(r == next(m.begin(), 3));
        r = m.lower_bound(13);
        assert(r == next(m.begin(), 4));
        r = m.lower_bound(15);
        assert(r == next(m.begin(), 5));
        r = m.lower_bound(17);
        assert(r == next(m.begin(), 6));
        r = m.lower_bound(19);
        assert(r == next(m.begin(), 7));
        r = m.lower_bound(4);
        assert(r == next(m.begin(), 0));
        r = m.lower_bound(6);
        assert(r == next(m.begin(), 1));
        r = m.lower_bound(8);
        assert(r == next(m.begin(), 2));
        r = m.lower_bound(10);
        assert(r == next(m.begin(), 3));
        r = m.lower_bound(12);
        assert(r == next(m.begin(), 4));
        r = m.lower_bound(14);
        assert(r == next(m.begin(), 5));
        r = m.lower_bound(16);
        assert(r == next(m.begin(), 6));
        r = m.lower_bound(18);
        assert(r == next(m.begin(), 7));
        r = m.lower_bound(20);
        assert(r == next(m.begin(), 8));
    }
}
