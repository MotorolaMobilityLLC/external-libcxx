//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11

// <experimental/any>

// template <class ValueType>
// ValueType const any_cast(any const&);
//
// template <class ValueType>
// ValueType any_cast(any &);
//
// template <class ValueType>
// ValueType any_cast(any &&);

#include <experimental/any>
#include <type_traits>
#include <cassert>

#include "any_helpers.h"
#include "count_new.hpp"
#include "test_macros.h"

using std::experimental::any;
using std::experimental::any_cast;
using std::experimental::bad_any_cast;


// Test that the operators are NOT marked noexcept.
void test_cast_is_not_noexcept() {
    any a;
    static_assert(!noexcept(any_cast<int>(static_cast<any&>(a))), "");
    static_assert(!noexcept(any_cast<int>(static_cast<any const&>(a))), "");
    static_assert(!noexcept(any_cast<int>(static_cast<any &&>(a))), "");
}

// Test that the return type of any_cast is correct.
void test_cast_return_type() {
    any a;
    static_assert(std::is_same<decltype(any_cast<int>(a)), int>::value, "");
    static_assert(std::is_same<decltype(any_cast<int const>(a)), int>::value, "");
    static_assert(std::is_same<decltype(any_cast<int&>(a)), int&>::value, "");
    static_assert(std::is_same<decltype(any_cast<int const&>(a)), int const&>::value, "");

    //static_assert(std::is_same<decltype(any_cast<int&&>(a)), int&&>::value, "");
    //static_assert(std::is_same<decltype(any_cast<int const&&>(a)), int const&&>::value, "");

    static_assert(std::is_same<decltype(any_cast<int>(std::move(a))), int>::value, "");
    static_assert(std::is_same<decltype(any_cast<int const>(std::move(a))), int>::value, "");
    static_assert(std::is_same<decltype(any_cast<int&>(std::move(a))), int&>::value, "");
    static_assert(std::is_same<decltype(any_cast<int const&>(std::move(a))), int const&>::value, "");

    //static_assert(std::is_same<decltype(any_cast<int&&>(std::move(a))), int&&>::value, "");
    //static_assert(std::is_same<decltype(any_cast<int const&&>(std::move(a))), int const&&>::value, "");

    any const& ca = a;
    static_assert(std::is_same<decltype(any_cast<int>(ca)), int>::value, "");
    static_assert(std::is_same<decltype(any_cast<int const>(ca)), int>::value, "");
    static_assert(std::is_same<decltype(any_cast<int const&>(ca)), int const&>::value, "");

    //static_assert(std::is_same<decltype(any_cast<int const&&>(ca)), int const&&>::value, "");
}

template <class Type, class ConstT = Type>
void checkThrows(any& a)
{
#if !defined(TEST_HAS_NO_EXCEPTIONS)
    try {
        any_cast<Type>(a);
        assert(false);
    } catch (bad_any_cast const &) {
            // do nothing
    } catch (...) {
        assert(false);
    }

    try {
        any_cast<ConstT>(static_cast<any const&>(a));
        assert(false);
    } catch (bad_any_cast const &) {
            // do nothing
    } catch (...) {
        assert(false);
    }

    try {
        any_cast<Type>(static_cast<any&&>(a));
        assert(false);
    } catch (bad_any_cast const &) {
            // do nothing
    } catch (...) {
        assert(false);
    }
#endif
}

void test_cast_empty() {
    // None of these operations should allocate.
    DisableAllocationGuard g; ((void)g);
    any a;
    checkThrows<int>(a);
}

template <class Type>
void test_cast_to_reference() {
    assert(Type::count == 0);
    Type::reset();
    {
        any a((Type(42)));
        any const& ca = a;
        assert(Type::count == 1);
        assert(Type::copied == 0);
        assert(Type::moved == 1);

        // Try a cast to a bad type.
        // NOTE: Type cannot be an int.
        checkThrows<int>(a);
        checkThrows<int&, int const&>(a);
        checkThrows<Type*, Type const*>(a);
        checkThrows<Type const*>(a);

        // Check getting a type by reference from a non-const lvalue any.
        {
            Type& v = any_cast<Type&>(a);
            assert(v.value == 42);

            Type const &cv = any_cast<Type const&>(a);
            assert(&cv == &v);
        }
        // Check getting a type by reference from a const lvalue any.
        {
            Type const& v = any_cast<Type const&>(ca);
            assert(v.value == 42);

            Type const &cv = any_cast<Type const&>(ca);
            assert(&cv == &v);
        }
        // Check getting a type by reference from a non-const rvalue
        {
            Type& v = any_cast<Type&>(std::move(a));
            assert(v.value == 42);

            Type const &cv = any_cast<Type const&>(std::move(a));
            assert(&cv == &v);
        }
        // Check getting a type by reference from a const rvalue any.
        {
            Type const& v = any_cast<Type const&>(std::move(ca));
            assert(v.value == 42);

            Type const &cv = any_cast<Type const&>(std::move(ca));
            assert(&cv == &v);
        }

        // Check that the original object hasn't been changed.
        assertContains<Type>(a, 42);

        // Check that no objects have been created/copied/moved.
        assert(Type::count == 1);
        assert(Type::copied == 0);
        assert(Type::moved == 1);
    }
    assert(Type::count == 0);
}

template <class Type>
void test_cast_to_value() {
    assert(Type::count == 0);
    Type::reset();
    {
        any a((Type(42)));
        any const& ca = a;
        assert(Type::count == 1);
        assert(Type::copied == 0);
        assert(Type::moved == 1);

        // Try a cast to a bad type.
        // NOTE: Type cannot be an int.
        checkThrows<int>(a);
        checkThrows<int&, int const&>(a);
        checkThrows<Type*, Type const*>(a);
        checkThrows<Type const*>(a);

        Type::reset(); // NOTE: reset does not modify Type::count
        // Check getting Type by value from a non-const lvalue any.
        // This should cause the non-const copy constructor to be called.
        {
            Type t = any_cast<Type>(a);

            assert(Type::count == 2);
            assert(Type::copied == 1);
            assert(Type::const_copied == 0);
            assert(Type::non_const_copied == 1);
            assert(Type::moved == 0);
            assert(t.value == 42);
        }
        assert(Type::count == 1);
        Type::reset();
        // Check getting const Type by value from a non-const lvalue any.
        // This should cause the const copy constructor to be called.
        {
            Type t = any_cast<Type const>(a);

            assert(Type::count == 2);
            assert(Type::copied == 1);
            assert(Type::const_copied == 1);
            assert(Type::non_const_copied == 0);
            assert(Type::moved == 0);
            assert(t.value == 42);
        }
        assert(Type::count == 1);
        Type::reset();
        // Check getting Type by value from a non-const lvalue any.
        // This should cause the const copy constructor to be called.
        {
            Type t = any_cast<Type>(static_cast<any const&>(a));

            assert(Type::count == 2);
            assert(Type::copied == 1);
            assert(Type::const_copied == 1);
            assert(Type::non_const_copied == 0);
            assert(Type::moved == 0);
            assert(t.value == 42);
        }
        assert(Type::count == 1);
        Type::reset();
        // Check getting Type by value from a non-const rvalue any.
        // This should cause the non-const copy constructor to be called.
        {
            Type t = any_cast<Type>(static_cast<any &&>(a));

            assert(Type::count == 2);
            assert(Type::copied == 1);
            assert(Type::const_copied == 0);
            assert(Type::non_const_copied == 1);
            assert(Type::moved == 0);
            assert(t.value == 42);
        }
        assert(Type::count == 1);
        Type::reset();
        // Check getting const Type by value from a non-const rvalue any.
        // This should cause the const copy constructor to be called.
        {
            Type t = any_cast<Type const>(static_cast<any &&>(a));

            assert(Type::count == 2);
            assert(Type::copied == 1);
            assert(Type::const_copied == 1);
            assert(Type::non_const_copied == 0);
            assert(Type::moved == 0);
            assert(t.value == 42);
        }
        assert(Type::count == 1);
        Type::reset();
        // Check getting Type by value from a const rvalue any.
        // This should cause the const copy constructor to be called.
        {
            Type t = any_cast<Type>(static_cast<any const&&>(a));

            assert(Type::count == 2);
            assert(Type::copied == 1);
            assert(Type::const_copied == 1);
            assert(Type::non_const_copied == 0);
            assert(Type::moved == 0);
            assert(t.value == 42);
        }
        // Ensure we still only have 1 Type object alive.
        assert(Type::count == 1);

        // Check that the original object hasn't been changed.
        assertContains<Type>(a, 42);
    }
    assert(Type::count == 0);
}

// Even though you can't get a non-copyable class into std::any
// the standard requires that these overloads compile and function.
void test_non_copyable_ref() {
    struct no_copy
    {
        no_copy() {}
        no_copy(no_copy &&) {}
    private:
        no_copy(no_copy const &);
    };

    any a;
    checkThrows<no_copy &, no_copy const&>(a);
    checkThrows<no_copy const&>(a);
    assertEmpty(a);
}

int main() {
    test_cast_is_not_noexcept();
    test_cast_return_type();
    test_cast_empty();
    test_cast_to_reference<small>();
    test_cast_to_reference<large>();
    test_cast_to_value<small>();
    test_cast_to_value<large>();
    test_non_copyable_ref();
}
