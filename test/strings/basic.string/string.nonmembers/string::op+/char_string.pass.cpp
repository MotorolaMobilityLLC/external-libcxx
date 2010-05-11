//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// template<class charT, class traits, class Allocator> 
//   basic_string<charT,traits,Allocator> 
//   operator+(charT lhs, const basic_string<charT,traits,Allocator>& rhs);

// template<class charT, class traits, class Allocator> 
//   basic_string<charT,traits,Allocator>&&
//   operator+(charT lhs, basic_string<charT,traits,Allocator>&& rhs);

#include <string>
#include <cassert>

template <class S>
void
test0(typename S::value_type lhs, const S& rhs, const S& x)
{
    assert(lhs + rhs == x);
}

#ifdef _LIBCPP_MOVE

template <class S>
void
test1(typename S::value_type lhs, S&& rhs, const S& x)
{
    assert(lhs + move(rhs) == x);
}

#endif

typedef std::string S;

int main()
{
    test0('a', S(""), S("a"));
    test0('a', S("12345"), S("a12345"));
    test0('a', S("1234567890"), S("a1234567890"));
    test0('a', S("12345678901234567890"), S("a12345678901234567890"));

#ifdef _LIBCPP_MOVE

    test1('a', S(""), S("a"));
    test1('a', S("12345"), S("a12345"));
    test1('a', S("1234567890"), S("a1234567890"));
    test1('a', S("12345678901234567890"), S("a12345678901234567890"));

#endif
}
