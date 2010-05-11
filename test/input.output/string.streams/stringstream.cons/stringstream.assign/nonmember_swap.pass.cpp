//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <sstream>

// template <class charT, class traits = char_traits<charT>, class Allocator = allocator<charT> >
// class basic_stringstream

// template <class charT, class traits, class Allocator> 
//   void
//   swap(basic_stringstream<charT, traits, Allocator>& x, 
//        basic_stringstream<charT, traits, Allocator>& y);

#include <sstream>
#include <cassert>

int main()
{
    {
        std::stringstream ss0(" 123 456 ");
        std::stringstream ss;
        swap(ss, ss0);
        assert(ss.rdbuf() != 0);
        assert(ss.good());
        assert(ss.str() == " 123 456 ");
        int i = 0;
        ss >> i;
        assert(i == 123);
        ss >> i;
        assert(i == 456);
        ss << i << ' ' << 123;
        assert(ss.str() == "456 1236 ");
        ss0 << i << ' ' << 123;
        assert(ss0.str() == "456 123");
    }
    {
        std::wstringstream ss0(L" 123 456 ");
        std::wstringstream ss;
        swap(ss, ss0);
        assert(ss.rdbuf() != 0);
        assert(ss.good());
        assert(ss.str() == L" 123 456 ");
        int i = 0;
        ss >> i;
        assert(i == 123);
        ss >> i;
        assert(i == 456);
        ss << i << ' ' << 123;
        assert(ss.str() == L"456 1236 ");
        ss0 << i << ' ' << 123;
        assert(ss0.str() == L"456 123");
    }
}
