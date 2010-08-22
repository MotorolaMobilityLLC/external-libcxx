//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <memory>

// template <class T>
// struct pointer_traits<T*>
// {
//     static pointer pointer_to(<details>);
//     ...
// };

#include <memory>
#include <cassert>

int main()
{
    {
        int i = 0;
        int* a = std::pointer_traits<int*>::pointer_to(i);
        assert(a = &i);
    }
    {
        (std::pointer_traits<void*>::element_type)0;
    }
}
