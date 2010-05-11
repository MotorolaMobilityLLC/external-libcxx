//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <fstream>

// template <class charT, class traits = char_traits<charT> >
// class basic_fstream

// basic_fstream& operator=(basic_fstream&& rhs);

#include <fstream>
#include <cassert>

int main()
{
#ifdef _LIBCPP_MOVE
    {
        std::fstream fso("test.dat", std::ios_base::in | std::ios_base::out
                                                       | std::ios_base::trunc);
        std::fstream fs;
        fs = move(fso);
        double x = 0;
        fs << 3.25;
        fs.seekg(0);
        fs >> x;
        assert(x == 3.25);
    }
    std::remove("test.dat");
    {
        std::wfstream fso("test.dat", std::ios_base::in | std::ios_base::out
                                                        | std::ios_base::trunc);
        std::wfstream fs;
        fs = move(fso);
        double x = 0;
        fs << 3.25;
        fs.seekg(0);
        fs >> x;
        assert(x == 3.25);
    }
    std::remove("test.dat");
#endif
}
