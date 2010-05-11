//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <istream>

// template <class charT, class traits, class T> 
//   basic_istream<charT, traits>& 
//   operator>>(basic_istream<charT, traits>&& is, T& x);

#include <istream>
#include <cassert>

#ifdef _LIBCPP_MOVE

template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    typedef std::basic_string<CharT> string_type;
    typedef std::basic_streambuf<CharT> base;
private:
    string_type str_;
public:

    testbuf() {}
    testbuf(const string_type& str)
        : str_(str)
    {
        base::setg(const_cast<CharT*>(str_.data()),
                   const_cast<CharT*>(str_.data()),
                   const_cast<CharT*>(str_.data()) + str_.size());
    }

    CharT* eback() const {return base::eback();}
    CharT* gptr() const {return base::gptr();}
    CharT* egptr() const {return base::egptr();}
};

#endif

int main()
{
#ifdef _LIBCPP_MOVE
    {
        testbuf<char> sb("   123");
        int i = 0;
        std::istream(&sb) >> i;
        assert(i == 123);
    }
    {
        testbuf<wchar_t> sb(L"   123");
        int i = 0;
        std::wistream(&sb) >> i;
        assert(i == 123);
    }
#endif
}
