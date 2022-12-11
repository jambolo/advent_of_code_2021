// Advent of Code 2021
// Day 6

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using json = nlohmann::json;

static int constexpr NUMBER_OF_DAYS = 256;
static int constexpr NUMBER_OF_AGES = 9;    // 0 - 8

struct extract
{
    char c;
    extract(char c) : c(c) { }
};

std::istream& operator >> (std::istream& in, extract e)
{
    // Skip leading whitespace IFF user is not asking to extract a whitespace character
    if (!std::isspace(e.c))
        in >> std::ws;

    // Attempt to get the specific character
    if (in.peek() == e.c)
        in.get();
    else
        in.setstate(std::ios::failbit);

    return in;
}

static void loadAges(std::ifstream& input, std::vector<int64_t>& byAge);

int main(int argc, char** argv)
{
    std::ifstream input("day06-input.txt");
    if (!input.is_open())
        exit(1);

    // Load picked numbers

    std::vector<int64_t> byAge(NUMBER_OF_AGES, 0);
    loadAges(input, byAge);

//    std::cerr << "Start: (" << std::accumulate(byAge.begin(), byAge.end(), 0) << ") " << json(byAge) << std::endl;

    for (int day = 0; day < NUMBER_OF_DAYS; ++day)
    {
        int iBirth = (day + 0) % byAge.size();

        // Decrement

        int iReset  = (day + 7) % byAge.size();

        int64_t nBirths = byAge[iBirth];
        byAge[iReset] += nBirths;

        std::vector<int64_t> byAgeAligned(byAge.size(), 0);
        for (int i = 0; i < byAgeAligned.size(); ++i)
        {
            byAgeAligned[i] = byAge[(iBirth + 1 + i) % byAgeAligned.size()];
        }

//        std::cerr << "At the end of day " << day + 1 << ": (" << std::accumulate(byAge.begin(), byAge.end(), int64_t(0)) << ") " << json(byAgeAligned) << std::endl;
    }

    std::cout << "The number of fish after " << NUMBER_OF_DAYS << " days is " << std::accumulate(byAge.begin(), byAge.end(), int64_t(0)) << std::endl;

    return 0;
}

static void loadAges(std::ifstream & input, std::vector<int64_t>& byAge)
{
    std::string line;
    std::getline(input, line);
    if (input.fail())
        return;

    std::istringstream lineStream(line);
    while (!lineStream.fail())
    {
        int age;
        lineStream >> age;
        if (!lineStream.fail())
            ++byAge[age];
        lineStream >> extract(',');
    }
}