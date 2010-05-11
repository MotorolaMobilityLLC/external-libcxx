//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <locale>

// class moneypunct_byname<charT, International>

// string grouping() const;

#include <locale>
#include <limits>
#include <cassert>

class Fnf
    : public std::moneypunct_byname<char, false>
{
public:
    explicit Fnf(const std::string& nm, std::size_t refs = 0)
        : std::moneypunct_byname<char, false>(nm, refs) {}
};

class Fnt
    : public std::moneypunct_byname<char, true>
{
public:
    explicit Fnt(const std::string& nm, std::size_t refs = 0)
        : std::moneypunct_byname<char, true>(nm, refs) {}
};

class Fwf
    : public std::moneypunct_byname<wchar_t, false>
{
public:
    explicit Fwf(const std::string& nm, std::size_t refs = 0)
        : std::moneypunct_byname<wchar_t, false>(nm, refs) {}
};

class Fwt
    : public std::moneypunct_byname<wchar_t, true>
{
public:
    explicit Fwt(const std::string& nm, std::size_t refs = 0)
        : std::moneypunct_byname<wchar_t, true>(nm, refs) {}
};

int main()
{
    {
        Fnf f("C", 1);
        assert(f.grouping() == "");
    }
    {
        Fnt f("C", 1);
        assert(f.grouping() == "");
    }
    {
        Fwf f("C", 1);
        assert(f.grouping() == "");
    }
    {
        Fwt f("C", 1);
        assert(f.grouping() == "");
    }

    {
        Fnf f("en_US", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fnt f("en_US", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fwf f("en_US", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fwt f("en_US", 1);
        assert(f.grouping() == "\3\3");
    }

    {
        Fnf f("fr_FR", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fnt f("fr_FR", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fwf f("fr_FR", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fwt f("fr_FR", 1);
        assert(f.grouping() == "\3\3");
    }

    {
        Fnf f("ru_RU", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fnt f("ru_RU", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fwf f("ru_RU", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fwt f("ru_RU", 1);
        assert(f.grouping() == "\3\3");
    }

    {
        Fnf f("zh_CN", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fnt f("zh_CN", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fwf f("zh_CN", 1);
        assert(f.grouping() == "\3\3");
    }
    {
        Fwt f("zh_CN", 1);
        assert(f.grouping() == "\3\3");
    }
}
