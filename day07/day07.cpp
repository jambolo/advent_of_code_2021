// Advent of Code 2021
// Day 7

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

//#define PART1 1

using json = nlohmann::json;

int main(int argc, char** argv)
{
    std::vector<int> positions;
    std::ifstream input("day07-input.txt");
    if (!input.is_open())
        exit(1);

    while (!input.fail())
    {
        int p;
        input >> p;
        if (!input.fail())
            positions.push_back(p);
        input >> expect(',');
    }

//    std::cerr << json(positions) << std::endl;

    std::sort(positions.begin(), positions.end());

#if defined(PART1)
    int best_sum = (positions.back() - positions.front())* positions.size();
#else
    int best_sum = (positions.back() - positions.front()) * (positions.back() - positions.front()) * positions.size() / 2;
#endif
    int best_x = 0;
    for (int x = positions.front(); x <= positions.back(); ++x)
    {
        int sum = 0;
        for (int p : positions)
        {
#if defined(PART1)
            sum += abs(p - x);
#else
            int d = abs(p - x);
            sum += d * (d + 1) / 2;
#endif // defined(PART1)
        }
        if (sum < best_sum)
        {
            best_x = x;
            best_sum = sum;
        }
//        std::cerr << "x = " << x << ", sum = " << sum << ", best_x = " << best_x << ", best_sum = " << best_sum << std::endl;
    }

    std::cout << "best_x = " << best_x << ", best_sum = " << best_sum << std::endl;


    return 0;
}
