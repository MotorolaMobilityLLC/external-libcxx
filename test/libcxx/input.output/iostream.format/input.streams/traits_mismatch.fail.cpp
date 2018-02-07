//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <istream>

// template <class charT, class traits = char_traits<charT> >
// class basic_istream;

// The char type of the stream and the char_type of the traits have to match

#include <istream>
#include <type_traits>
#include <cassert>

struct test_istream
    : public std::basic_istream<char, std::char_traits<wchar_t> > {};


int main()
{
//  expected-error-re@ios:* {{static_assert failed{{.*}} "traits_type::char_type must be the same type as CharT"}}
}

