// Advent of Code 2021
// Day 10

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

#define PART    2

using json = nlohmann::json;

static char constexpr FILE_NAME[] = "day11-input.txt";
static int constexpr NUMBER_OF_STEPS = 100;
static int constexpr WIDTH = 10;
static int constexpr HEIGHT = 10;
static int constexpr FLASHED = WIDTH * HEIGHT * WIDTH * HEIGHT + 1; // Sentinel value for cells that have flashed

static void loadInput(char const* name, std::vector<std::string> & lines);
static void bumpCell(int& cell);
static void bumpNeighbors(std::array<std::array<int, WIDTH>, HEIGHT>& map, int r, int c);
static void printMap(std::array<std::array<int, WIDTH>, HEIGHT> const & map);


int main(int argc, char** argv)
{
    // Load the input
    std::vector<std::string> lines;
    loadInput(FILE_NAME, lines);

    if (lines.size() != HEIGHT)
        throw std::runtime_error("Heigth mismatch");

//    std::cerr << "Input: " << json(lines) << std::endl;

    // Build the map
    std::array<std::array<int, WIDTH>, HEIGHT> map;
    for (int r = 0; r < HEIGHT; ++r)
        for (int c = 0; c < WIDTH; ++c)
            map[r][c] = lines[r][c] - '0';

//    std::cerr << "Map: " << json(map) << std::endl;

    // Go
#if PART == 1
    int nFlashed = 0;
    for (int step = 0; step < NUMBER_OF_STEPS; ++step)
#else
    int step = 1;
    while (true)
#endif
    {
        // Bump the energy level
        for (int r = 0; r < HEIGHT; ++r)
            for (int c = 0; c < WIDTH; ++c)
                map[r][c] += 1;

        // Flash
        bool flashed;
        do
        {
            flashed = false;    // Nobody has flashed yet

            for (int r = 0; r < HEIGHT; ++r)
            {
                for (int c = 0; c < WIDTH; ++c)
                {
                    if (map[r][c] >= 10 && map[r][c] != FLASHED)
                    {
                        map[r][c] = FLASHED;
#if PART == 1
                        ++nFlashed;
#endif
                        bumpNeighbors(map, r, c);
                        flashed = true;
                    }
                }
            }
        } while (flashed);

        // Reset flashed cells
#if PART == 2
        int nFlashedInOneStep = 0;
#endif
        for (int r = 0; r < HEIGHT; ++r)
        {
            for (int c = 0; c < WIDTH; ++c)
            {
                if (map[r][c] == FLASHED)
                {
                    map[r][c] = 0;
#if PART == 2
                    ++nFlashedInOneStep;
#endif
                }
            }
        }
#if PART == 2
        if (nFlashedInOneStep == WIDTH * HEIGHT)
        {
            std::cout << "Synchronized in step " << step << std::endl;
            break;
        }
        ++step;
#endif
//        printMap(map);
    }

#if PART == 1
    std::cout << nFlashed << " cells flashed." << std::endl;
#endif

    return 0;
}

static void loadInput(char const * name, std::vector<std::string>& lines)
{
    std::ifstream input(name);
    if (!input.is_open())
    {
        std::cerr << "Unable to open for reading '" << name << "'" << std::endl;
        exit(1);
    }

    while (!input.fail())
    {
        // Read a line
        std::string line;
        std::getline(input, line);
        if (input.fail())
            break;
        lines.emplace_back(line);
    }
}

static void bumpCell(int& cell)
{
    if (cell != FLASHED)
        ++cell;
}

static void bumpNeighbors(std::array<std::array<int, WIDTH>, HEIGHT>& map, int r, int c)
{
    if (r > 0)
    {
        if (c > 0)
            bumpCell(map[r - 1][c - 1]);
        bumpCell(map[r - 1][c]);
        if (c < WIDTH - 1)
            bumpCell(map[r - 1][c + 1]);
    }

    if (c > 0)
        bumpCell(map[r][c - 1]);
    if (c < WIDTH - 1)
        bumpCell(map[r][c + 1]);

    if (r < HEIGHT - 1)
    {
        if (c > 0)
            bumpCell(map[r + 1][c - 1]);
        bumpCell(map[r + 1][c]);
        if (c < WIDTH - 1)
            bumpCell(map[r + 1][c + 1]);
    }
}

void printMap(std::array<std::array<int, WIDTH>, HEIGHT> const& map)
{
    for (int r = 0; r < HEIGHT; ++r)
    {
        std::string row(WIDTH, '.');
        for (int c = 0; c < WIDTH; ++c)        {
            int level = map[r][c];
            if (level == FLASHED)
                row[c] = '#';
            else if (level >= 10)
                row[c] = 'X';
            else
                row[c] = level + '0';
        }
        std::cerr << row << std::endl;
    }
    std::cerr << std::endl;
}
