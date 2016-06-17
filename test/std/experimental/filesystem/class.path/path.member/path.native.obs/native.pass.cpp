//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <experimental/filesystem>

// class path

// const string_type& native() const noexcept;

#include <experimental/filesystem>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "filesystem_test_helper.hpp"

namespace fs = std::experimental::filesystem;

int main()
{
  using namespace fs;
  const char* const value = "hello world";
  path p(value);
  { // Check signature
    ASSERT_SAME_TYPE(path::string_type const&, decltype(p.native()));
    ASSERT_NOEXCEPT(p.native());
  }
  { // native() is tested elsewhere
    path p(value);
    assert(p.native() == value);
  }
}
