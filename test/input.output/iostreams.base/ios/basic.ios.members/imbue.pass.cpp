//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// locale imbue(const locale& loc);

#include <ios>
#include <streambuf>
#include <cassert>

struct testbuf
    : public std::streambuf
{
};

bool f1_called = false;
bool f2_called = false;
bool f3_called = false;

void f1(std::ios_base::event ev, std::ios_base& stream, int index)
{
    if (ev == std::ios_base::imbue_event)
    {
        assert(!f1_called);
        assert( f2_called);
        assert( f3_called);
        assert(stream.getloc().name() == "en_US");
        assert(index == 4);
        f1_called = true;
    }
}

void f2(std::ios_base::event ev, std::ios_base& stream, int index)
{
    if (ev == std::ios_base::imbue_event)
    {
        assert(!f1_called);
        assert(!f2_called);
        assert( f3_called);
        assert(stream.getloc().name() == "en_US");
        assert(index == 5);
        f2_called = true;
    }
}

void f3(std::ios_base::event ev, std::ios_base& stream, int index)
{
    if (ev == std::ios_base::imbue_event)
    {
        assert(!f1_called);
        assert(!f2_called);
        assert(!f3_called);
        assert(stream.getloc().name() == "en_US");
        assert(index == 6);
        f3_called = true;
    }
}

int main()
{
    {
        std::ios ios(0);
        ios.register_callback(f1, 4);
        ios.register_callback(f2, 5);
        ios.register_callback(f3, 6);
        std::locale l = ios.imbue(std::locale("en_US"));
        assert(l.name() == std::string("C"));
        assert(ios.getloc().name() == std::string("en_US"));
        assert(f1_called);
        assert(f2_called);
        assert(f3_called);
    }
    f1_called = false;
    f2_called = false;
    f3_called = false;
    {
        testbuf sb;
        std::ios ios(&sb);
        ios.register_callback(f1, 4);
        ios.register_callback(f2, 5);
        ios.register_callback(f3, 6);
        std::locale l = ios.imbue(std::locale("en_US"));
        assert(l.name() == std::string("C"));
        assert(ios.getloc().name() == std::string("en_US"));
        assert(sb.getloc().name() == std::string("en_US"));
        assert(f1_called);
        assert(f2_called);
        assert(f3_called);
    }
}
