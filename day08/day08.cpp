// Advent of Code 2021
// Day 8

// Logic as follows
//  tttt
// u    v
// u    v
//  wwww
// x    y
// x    y
//  zzzz
// 
// 1 = v + y +                      size() == 2
// 7 = t + v + y                    size() == 3
// 4 = u + v + w + y +              size() == 4
// 8 = t + u + v + w + x + y + z    size() == 7
// 3 = t + v + w + y + z            size() == 5 && contains all of 1
// 5 = t + u + w + y + z            size() == 5 && !3 && contains all of 4 - 1
// 2 = t + v + w + x + z            size() == 5 && !3 && !5
// 6 = t + u + w + x + y + z        size() == 6 && !contains all of 1
// 9 = t + u + v + w + y + z        size() == 6 && contains all of 4
// 0 = t + u + v + x + y + z        size() == 6 && !6 && !8
// 
// t = 7 - 1
// u = 0 + 4 - 1 - 8
// v = 8 - 6
// w = 8 - 0
// x = 6 - 5
// y = 1 + 6 - 8
// z = 0 + 3 - 7 - 8

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

#include "common/expect.h"

#define PART1 1

using json = nlohmann::json;

static int constexpr COUNT_0 = 6;
static int constexpr COUNT_1 = 2;
static int constexpr COUNT_2 = 5;
static int constexpr COUNT_3 = 5;
static int constexpr COUNT_4 = 4;
static int constexpr COUNT_5 = 5;
static int constexpr COUNT_6 = 6;
static int constexpr COUNT_7 = 3;
static int constexpr COUNT_8 = 7;
static int constexpr COUNT_9 = 6;

static int count1478(std::vector<std::string> const& numbers);
static void deduceCodes(std::vector<std::string>& codes, std::vector<std::string> const & numbers);
static bool containsAllOf(std::string const& s, std::string const& t);
static std::string removeEachOf(std::string s, std::string const & remove);

int main(int argc, char** argv)
{
    int count = 0;
    int sum = 0;

    std::ifstream input("day08-input.txt");
    if (!input.is_open())
        exit(1);

    while (!input.fail())
    {
        // Read a line
        std::string line;
        std::getline(input, line);
        if (input.fail())
            break;

        std::vector<std::string> leftNumbers;
        std::vector<std::string> rightNumbers;
        std::vector<std::string> codes(10);

        // Parse the line
        std::istringstream lineStream(line);
        while (!lineStream.fail())
        {
            // Parse each number and split into right and left
            bool left = true;
            while (!lineStream.fail())
            {
                std::string p;
                lineStream >> p;
                if (lineStream.fail())
                    break;
                if (p == "|")
                {
                    left = false;
                    continue;
                }

                std::sort(p.begin(), p.end());
                if (left)
                {
                    leftNumbers.push_back(p);
                }
                else
                {
                    rightNumbers.push_back(p);
                }
            }
        }

        // Get the counts for the line
        count += count1478(rightNumbers);

        // Get the codes for each number
        deduceCodes(codes, leftNumbers);

//        std::cerr << "Codes: " << json(codes) << std::endl;

        int value = 0;
        for (std::string const & n : rightNumbers)
        {
            auto found = std::find(codes.begin(), codes.end(), n);
            int digit = (int)std::distance(codes.begin(), found);
            value = value * 10 + digit;
        }
        sum += value;
        std::cerr << "Value: " << value << std::endl;
    }
    std::cout << "1, 4, 7, 8 appear " << count << " times" << std::endl;
    std::cout << "Sum is " << sum << std::endl;
    return 0;
}

static int count1478(std::vector<std::string> const& numbers)
{
    int count = 0;
    for (auto const& p : numbers)
    {
        switch (p.size())
        {
        case COUNT_1:
        case COUNT_4:
        case COUNT_7:
        case COUNT_8:
            ++count;
            break;
        case COUNT_0:
        case COUNT_2:
        //case COUNT_3:
        //case COUNT_5:
        //case COUNT_6:
        //case COUNT_9:
            break;
        default:
            std::cerr << "Undecipherable input: " + p << std::endl;
            exit(1);
        }
    }
    return count;
}

static void deduceCodes(std::vector<std::string> & codes, std::vector<std::string> const & numbers)
{
    codes[1] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 2;
        });
    codes[4] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 4;
        });
    codes[7] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 3;
        });
    codes[8] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 7;
        });
    codes[3] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 5 && containsAllOf(s, codes[1]);
        });
    codes[5] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 5 && s != codes[3] && containsAllOf(s, removeEachOf(codes[4], codes[1]));
        });
    codes[2] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 5 && s != codes[3] && s != codes[5];
        });
    codes[6] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 6 && !containsAllOf(s, codes[1]);
        });
    codes[9] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 6 && containsAllOf(s, codes[4]);
        });
    codes[0] = *std::find_if(numbers.begin(), numbers.end(), [&codes](std::string const& s) {
        return s.size() == 6 && s != codes[6] && s != codes[9];
        });

}
static bool containsAllOf(std::string const& s, std::string const& test)
{
    for (char t : test)
    {
        if (s.find(t) == s.npos)
            return false;
    }
    return true;
}

static std::string removeEachOf(std::string s, std::string const& remove)
{
    for (char c : remove)
    {
        s.erase(std::remove(s.begin(), s.end(), c), s.end());
    }
    return s;
}
