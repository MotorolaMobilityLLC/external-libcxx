//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <set>

// class multiset

// template <class... Args>
//   iterator emplace_hint(const_iterator position, Args&&... args);

#include <set>
#include <cassert>

#include "../../Emplaceable.h"
#include "../../DefaultOnly.h"

int main()
{
#ifdef _LIBCPP_MOVE
    {
        typedef std::multiset<DefaultOnly> M;
        typedef M::iterator R;
        M m;
        assert(DefaultOnly::count == 0);
        R r = m.emplace_hint(m.cend());
        assert(r == m.begin());
        assert(m.size() == 1);
        assert(*m.begin() == DefaultOnly());
        assert(DefaultOnly::count == 1);

        r = m.emplace_hint(m.cbegin());
        assert(r == m.begin());
        assert(m.size() == 2);
        assert(*m.begin() == DefaultOnly());
        assert(DefaultOnly::count == 2);
    }
    assert(DefaultOnly::count == 0);
    {
        typedef std::multiset<Emplaceable> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace_hint(m.cend());
        assert(r == m.begin());
        assert(m.size() == 1);
        assert(*m.begin() == Emplaceable());
        r = m.emplace_hint(m.cend(), 2, 3.5);
        assert(r == next(m.begin()));
        assert(m.size() == 2);
        assert(*r == Emplaceable(2, 3.5));
        r = m.emplace_hint(m.cbegin(), 2, 3.5);
        assert(r == next(m.begin()));
        assert(m.size() == 3);
        assert(*r == Emplaceable(2, 3.5));
    }
    {
        typedef std::multiset<int> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace_hint(m.cend(), M::value_type(2));
        assert(r == m.begin());
        assert(m.size() == 1);
        assert(*r == 2);
    }
#endif
}
