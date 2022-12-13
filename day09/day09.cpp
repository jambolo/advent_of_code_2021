// Advent of Code 2021
// Day 9

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include "common/expect.h"

#define PART1 1

using json = nlohmann::json;

static char constexpr FILE_NAME[] = "day09-input.txt";
static int constexpr WIDTH = 100;
static int constexpr HEIGHT = 100;

static void loadInput(std::array<std::array<int, WIDTH>, HEIGHT> & heights);
static bool isLowPoint(std::array<std::array<int, WIDTH>, HEIGHT> const& heights, int r, int c);
static int fillBasin(std::array<std::array<int, WIDTH>, HEIGHT> & heights, int r, int c);
int main(int argc, char** argv)
{
    std::array<std::array<int, WIDTH>, HEIGHT> heights;

    // Load the input
    loadInput(heights);

    std::vector<std::pair<int, int>> lows;

    // Find the low points
    for (int r = 0; r < HEIGHT; ++r)
    {
        for (int c = 0; c < WIDTH; ++c)
        {
            if (isLowPoint(heights, r, c))
                lows.emplace_back(std::make_pair(r, c));
        }
    }

//    std::cerr << "Lows: " << json(lows) << std::endl;

    // Sum the risk levels

    int risk = 0;
    for (auto const& low : lows)
    {
        risk += heights[low.first][low.second] + 1;
    }

    std::cout << "Sum of risk levels = " << risk << std::endl;

    // Find and fill the basins
    std::vector<int> basinSizes;

    for (int r = 0; r < HEIGHT; ++r)
    {
        for (int c = 0; c < WIDTH; ++c)
        {
            if (heights[r][c] < 9)
            {
                int size = fillBasin(heights, r, c);
                basinSizes.push_back(size);
            }
        }
    }

    std::sort(basinSizes.begin(), basinSizes.end());
//    std::cerr << "Basin sizes: " << json(basinSizes) << std::endl;

    int64_t result = 1;
    for (auto p = basinSizes.rbegin(); p != basinSizes.rbegin() + 3; ++p)
    {
        result *= (int64_t)*p;
    }
    
    std::cout << "Product of 3 biggest basin sizes is " << result << std::endl;

    return 0;
}

static void loadInput(std::array<std::array<int, WIDTH>, HEIGHT> & heights)
{
    std::ifstream input(FILE_NAME);
    if (!input.is_open())
    {
        std::cerr << "Unable to open for reading '" << FILE_NAME << "'" << std::endl;
        exit(1);
    }

    int r = 0;
    while (!input.fail())
    {
        // Read a line
        std::string line;
        std::getline(input, line);
        if (input.fail())
            break;

        if (line.size() != WIDTH)
            throw std::runtime_error("Size of a line is not equal to WIDTH.");

        for (int c = 0; c < WIDTH; ++c)
        {
            heights[r][c] = line[c] - '0';
        }
        ++r;
    }
    if (r != HEIGHT)
        throw std::runtime_error("Height of the array is not equal to HEIGHT.");

//    std::cerr << "Heights: " << json(heights) << std::endl;
}

bool isLowPoint(std::array<std::array<int, WIDTH>, HEIGHT> const& heights, int r, int c)
{
    int h = heights[r][c];

    if (r - 1 >= 0 && heights[r - 1][c] <= h)
        return false;
    if (r + 1 < HEIGHT && heights[r + 1][c] <= h)
        return false;
    if (c - 1 >= 0 && heights[r][c - 1] <= h)
        return false;
    if (c + 1 < WIDTH && heights[r][c + 1] <= h)
        return false;
    return true;
}

static int fillBasin(std::array<std::array<int, WIDTH>, HEIGHT>& heights, int r, int c)
{
    int size = 0;

    std::deque<std::pair<int, int>> queue;
    queue.push_back(std::make_pair(r, c));

    while (!queue.empty())
    {
        std::pair<int, int> p = queue.front();
        queue.pop_front();

        int r = p.first;
        int c = p.second;

        if (heights[p.first][p.second] < 9)
        {
            heights[p.first][p.second] = 9;
            ++size;

            if (r - 1 >= 0 && heights[r - 1][c] < 9)
            {
                std::pair<int, int> adjacent{ r - 1, c };
                if (std::find(queue.begin(), queue.end(), adjacent) == queue.end())
                    queue.push_back(adjacent);
            }
            if (r + 1 < HEIGHT && heights[r + 1][c] < 9)
            {
                std::pair<int, int> adjacent{ r + 1, c };
                if (std::find(queue.begin(), queue.end(), adjacent) == queue.end())
                    queue.push_back(adjacent);
            }
            if (c - 1 >= 0 && heights[r][c - 1] < 9)
            {
                std::pair<int, int> adjacent{ r, c - 1 };
                if (std::find(queue.begin(), queue.end(), adjacent) == queue.end())
                    queue.push_back(adjacent);
            }
            if (c + 1 < WIDTH && heights[r][c + 1] < 9)
            {
                std::pair<int, int> adjacent{ r, c + 1 };
                if (std::find(queue.begin(), queue.end(), adjacent) == queue.end())
                    queue.push_back(adjacent);
            }
        }
    }
    return size;
}
