//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// void move(basic_ios&& rhs);

#include <ios>
#include <streambuf>
#include <cassert>

struct testbuf
    : public std::streambuf
{
};

struct testios
    : public std::ios
{
    testios() {}
    testios(std::streambuf* p) : std::ios(p) {}
    void move(std::ios& x) {std::ios::move(x);}
};

bool f1_called = false;
bool f2_called = false;

bool g1_called = false;
bool g2_called = false;
bool g3_called = false;

void f1(std::ios_base::event ev, std::ios_base& stream, int index)
{
    f1_called = true;
}

void f2(std::ios_base::event ev, std::ios_base& stream, int index)
{
    f2_called = true;
}

void g1(std::ios_base::event ev, std::ios_base& stream, int index)
{
    if (ev == std::ios_base::imbue_event)
    {
        assert(index == 7);
        g1_called = true;
    }
}

void g2(std::ios_base::event ev, std::ios_base& stream, int index)
{
    if (ev == std::ios_base::imbue_event)
    {
        assert(index == 8);
        g2_called = true;
    }
}

void g3(std::ios_base::event ev, std::ios_base& stream, int index)
{
    if (ev == std::ios_base::imbue_event)
    {
        assert(index == 9);
        g3_called = true;
    }
}

int main()
{
    testios ios1;
    testbuf sb2;
    std::ios ios2(&sb2);
    ios2.flags(std::ios::showpoint | std::ios::uppercase);
    ios2.precision(2);
    ios2.width(12);
    ios2.imbue(std::locale("fr_FR"));
    ios2.exceptions(std::ios::eofbit);
    ios2.setstate(std::ios::goodbit);
    ios2.register_callback(g1, 7);
    ios2.register_callback(g2, 8);
    ios2.register_callback(g3, 9);
    ios2.iword(0) = 4;
    ios2.iword(1) = 5;
    ios2.iword(2) = 6;
    ios2.iword(3) = 7;
    ios2.iword(4) = 8;
    ios2.iword(5) = 9;
    char d1, d2;
    ios2.pword(0) = &d1;
    ios2.pword(1) = &d2;
    ios2.tie((std::ostream*)2);
    ios2.fill('2');

    ios1.move(ios2);

    assert(ios1.rdstate() == std::ios::goodbit);
    assert(ios1.rdbuf() == 0);
    assert(ios1.flags() == (std::ios::showpoint | std::ios::uppercase));
    assert(ios1.precision() == 2);
    assert(ios1.width() == 12);
    assert(ios1.getloc().name() == "fr_FR");
    assert(ios1.exceptions() == std::ios::eofbit);
    assert(!f1_called);
    assert(!f2_called);
    assert(!g1_called);
    assert(!g2_called);
    assert(!g3_called);
    assert(ios1.iword(0) == 4);
    assert(ios1.iword(1) == 5);
    assert(ios1.iword(2) == 6);
    assert(ios1.iword(3) == 7);
    assert(ios1.iword(4) == 8);
    assert(ios1.iword(5) == 9);
    assert(ios1.pword(0) == &d1);
    assert(ios1.pword(1) == &d2);
    assert(ios1.tie() == (std::ostream*)2);
    assert(ios1.fill() == '2');
    ios1.imbue(std::locale("C"));
    assert(!f1_called);
    assert(!f2_called);
    assert(g1_called);
    assert(g2_called);
    assert(g3_called);

    assert(ios2.rdbuf() == &sb2);
    assert(ios2.tie() == 0);
}
