#if !defined(EXPECT_H_INCLUDED)
#define EXPECT_H_INCLUDED 1
#pragma once

#include <istream>
#include <cctype>

struct expect
{
    std::string expected;
    expect(std::string const& s) : expected(s) { }
    expect(char c) : expected(1, c) { }
};

std::istream& operator >> (std::istream& in, expect e);

#endif // !defined(EXPECT_H_INCLUDED)