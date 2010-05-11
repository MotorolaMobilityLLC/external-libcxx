//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>
// vector<bool>

// void reserve(size_type n);

#include <vector>
#include <cassert>

int main()
{
    {
        std::vector<bool> v;
        v.reserve(10);
        assert(v.capacity() >= 10);
    }
    {
        std::vector<bool> v(100);
        assert(v.capacity() >= 100);
        v.reserve(50);
        assert(v.size() == 100);
        assert(v.capacity() >= 100);
        v.reserve(150);
        assert(v.size() == 100);
        assert(v.capacity() >= 150);
    }
}
