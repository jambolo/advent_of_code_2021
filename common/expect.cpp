#include "expect.h"

std::istream& operator >> (std::istream& in, expect e)
{
    // Skip leading whitespace IFF user is not asking to extract a leading whitespace character
    if (!std::isspace(e.expected[0]))
        in >> std::ws;

    // Attempt to get the specific characters
    for (char c : e.expected)
    {
        if (in.peek() == c)
        {
            in.get();
        }
        else
        {
            in.setstate(std::ios::failbit);
            break;
        }
    }

    return in;
}
