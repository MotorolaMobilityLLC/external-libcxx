//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// test quick_exit and at_quick_exit

#include <cstdlib>
#include <type_traits>

void f();

int main()
{
    std::at_quick_exit(f);
    quick_exit(0);
}
