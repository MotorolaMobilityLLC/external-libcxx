//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <memory>

// unique_ptr

// Test unique_ptr move assignment

#include <memory>
#include <cassert>

// Can't copy from const lvalue

struct A
{
    static int count;
    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int main()
{
    {
    const std::unique_ptr<A[]> s(new A[3]);
    std::unique_ptr<A[]> s2;
    s2 = s;
    }
}
