//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <tuple>

// template <class... Types> class tuple;

// template<class... TTypes, class... UTypes>
//   bool
//   operator==(const tuple<TTypes...>& t, const tuple<UTypes...>& u);

#include <tuple>
#include <string>
#include <cassert>

int main()
{
    {
        typedef std::tuple<> T1;
        typedef std::tuple<> T2;
        const T1 t1;
        const T2 t2;
        assert(t1 == t2);
        assert(!(t1 != t2));
    }
    {
        typedef std::tuple<int> T1;
        typedef std::tuple<double> T2;
        const T1 t1(1);
        const T2 t2(1.1);
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<int> T1;
        typedef std::tuple<double> T2;
        const T1 t1(1);
        const T2 t2(1);
        assert(t1 == t2);
        assert(!(t1 != t2));
    }
    {
        typedef std::tuple<int, double> T1;
        typedef std::tuple<double, char> T2;
        const T1 t1(1, 2);
        const T2 t2(1, char(2));
        assert(t1 == t2);
        assert(!(t1 != t2));
    }
    {
        typedef std::tuple<int, double> T1;
        typedef std::tuple<double, char> T2;
        const T1 t1(1, 2);
        const T2 t2(1, char(3));
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<int, double> T1;
        typedef std::tuple<double, char> T2;
        const T1 t1(1, 2);
        const T2 t2(1.1, char(2));
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<int, double> T1;
        typedef std::tuple<double, char> T2;
        const T1 t1(1, 2);
        const T2 t2(1.1, char(3));
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<char, int, double> T1;
        typedef std::tuple<double, char, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1, 2, 3);
        assert(t1 == t2);
        assert(!(t1 != t2));
    }
    {
        typedef std::tuple<char, int, double> T1;
        typedef std::tuple<double, char, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1.1, 2, 3);
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<char, int, double> T1;
        typedef std::tuple<double, char, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1, 3, 3);
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<char, int, double> T1;
        typedef std::tuple<double, char, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1, 2, 4);
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<char, int, double> T1;
        typedef std::tuple<double, char, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1, 3, 2);
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<char, int, double> T1;
        typedef std::tuple<double, char, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1.1, 2, 2);
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<char, int, double> T1;
        typedef std::tuple<double, char, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1.1, 3, 3);
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
    {
        typedef std::tuple<char, int, double> T1;
        typedef std::tuple<double, char, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1.1, 3, 2);
        assert(!(t1 == t2));
        assert(t1 != t2);
    }
}
