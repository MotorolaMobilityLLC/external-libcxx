//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <streambuf>

// template <class charT, class traits = char_traits<charT> >
// class basic_streambuf;

// locale pubimbue(const locale& loc);
// locale getloc() const;

#include <streambuf>
#include <cassert>

template <class CharT>
struct test
    : public std::basic_streambuf<CharT>
{
    test() {}

    void imbue(const std::locale&)
    {
        assert(this->getloc().name() == "en_US.UTF-8");
    }
};

int main()
{
    {
        test<char> t;
        assert(t.getloc().name() == "C");
    }
    std::locale::global(std::locale("en_US.UTF-8"));
    {
        test<char> t;
        assert(t.getloc().name() == "en_US.UTF-8");
        assert(t.pubimbue(std::locale("fr_FR.UTF-8")).name() == "en_US.UTF-8");
        assert(t.getloc().name() == "fr_FR.UTF-8");
    }
}
