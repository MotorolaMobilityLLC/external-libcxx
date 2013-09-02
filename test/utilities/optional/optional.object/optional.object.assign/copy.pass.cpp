//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <optional>

// optional<T>& operator=(const optional<T>& rhs);

#include <optional>
#include <type_traits>
#include <cassert>

#if _LIBCPP_STD_VER > 11

struct X
{
    static bool throw_now;

    X() = default;
    X(const X&)
    {
        if (throw_now)
            throw 6;
    }
};

bool X::throw_now = false;

#endif  // _LIBCPP_STD_VER > 11

int main()
{
#if _LIBCPP_STD_VER > 11
    {
        std::optional<int> opt;
        constexpr std::optional<int> opt2;
        opt = opt2;
        static_assert(static_cast<bool>(opt2) == false, "");
        assert(static_cast<bool>(opt) == static_cast<bool>(opt2));
    }
    {
        std::optional<int> opt;
        constexpr std::optional<int> opt2(2);
        opt = opt2;
        static_assert(static_cast<bool>(opt2) == true, "");
        static_assert(*opt2 == 2, "");
        assert(static_cast<bool>(opt) == static_cast<bool>(opt2));
        assert(*opt == *opt2);
    }
    {
        std::optional<int> opt(3);
        constexpr std::optional<int> opt2;
        opt = opt2;
        static_assert(static_cast<bool>(opt2) == false, "");
        assert(static_cast<bool>(opt) == static_cast<bool>(opt2));
    }
    {
        std::optional<int> opt(3);
        constexpr std::optional<int> opt2(2);
        opt = opt2;
        static_assert(static_cast<bool>(opt2) == true, "");
        static_assert(*opt2 == 2, "");
        assert(static_cast<bool>(opt) == static_cast<bool>(opt2));
        assert(*opt == *opt2);
    }
    {
        std::optional<X> opt;
        std::optional<X> opt2(X{});
        assert(static_cast<bool>(opt2) == true);
        try
        {
            X::throw_now = true;
            opt = opt2;
            assert(false);
        }
        catch (int i)
        {
            assert(i == 6);
            assert(static_cast<bool>(opt) == false);
        }
    }
#endif  // _LIBCPP_STD_VER > 11
}
