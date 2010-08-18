//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <regex>

// class regex_token_iterator<BidirectionalIterator, charT, traits>

// regex_token_iterator(BidirectionalIterator a, BidirectionalIterator b,
//                      const regex_type& re, int submatch = 0,
//                      regex_constants::match_flag_type m =
//                                              regex_constants::match_default);

#include <regex>
#include <cassert>

int main()
{
    {
        std::regex phone_numbers("\\d{3}-\\d{4}");
        const char phone_book[] = "start 555-1234, 555-2345, 555-3456 end";
        std::cregex_token_iterator i(std::begin(phone_book), std::end(phone_book),
                                     phone_numbers, -1);
        assert(i != std::cregex_token_iterator());
        assert(i->str() == "start ");
        ++i;
        assert(i != std::cregex_token_iterator());
        assert(i->str() == ", ");
        ++i;
        assert(i != std::cregex_token_iterator());
        assert(i->str() == ", ");
        ++i;
        assert(i != std::cregex_token_iterator());
        assert(i->str() == " end");
        ++i;
        assert(i == std::cregex_token_iterator());
    }
    {
        std::regex phone_numbers("\\d{3}-\\d{4}");
        const char phone_book[] = "start 555-1234, 555-2345, 555-3456 end";
        std::cregex_token_iterator i(std::begin(phone_book), std::end(phone_book),
                                     phone_numbers);
        assert(i != std::cregex_token_iterator());
        assert(i->str() == "555-1234");
        ++i;
        assert(i != std::cregex_token_iterator());
        assert(i->str() == "555-2345");
        ++i;
        assert(i != std::cregex_token_iterator());
        assert(i->str() == "555-3456");
        ++i;
        assert(i == std::cregex_token_iterator());
    }
    {
        std::regex phone_numbers("\\d{3}-(\\d{4})");
        const char phone_book[] = "start 555-1234, 555-2345, 555-3456 end";
        std::cregex_token_iterator i(std::begin(phone_book), std::end(phone_book),
                                     phone_numbers, 1);
        assert(i != std::cregex_token_iterator());
        assert(i->str() == "1234");
        ++i;
        assert(i != std::cregex_token_iterator());
        assert(i->str() == "2345");
        ++i;
        assert(i != std::cregex_token_iterator());
        assert(i->str() == "3456");
        ++i;
        assert(i == std::cregex_token_iterator());
    }
}
