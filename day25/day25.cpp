// Advent of Code 2021
// Day 25

#include "common/expect.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

//#define TEST    1
#define PART_1  1

#if defined(TEST)
static char constexpr FILE_NAME[] = "day25-test.txt";
#else
static char constexpr FILE_NAME[] = "day25-input.txt";
#endif

#if defined(TEST)
static size_t constexpr WIDTH   = 10;
static size_t constexpr HEIGHT  =  9;
#else
static size_t constexpr WIDTH   = 139;
static size_t constexpr HEIGHT  = 137;
#endif

static void readFile(char const* name, std::vector<std::string> & lines);
static void printMap(std::vector<std::string> const& map, int i);

int main(int argc, char** argv)
{
    // Read the input
    std::vector<std::string> lines;
    readFile(FILE_NAME, lines);

    int iterations = 0;
    int moveCount;

    do
    {
        moveCount = 0;
        std::vector<std::string> newLines = lines;

        // Move right
        for (size_t i = 0; i < HEIGHT; ++i)
        {
            std::string & line = lines[i];
            std::string& newLine = newLines[i];
            for (size_t j = 0; j < WIDTH; ++j)
            {
                size_t j1 = (j + 1) % WIDTH;
                if (line[j] == '>' && line[j1] == '.')
                {
                    newLine[j] = '.';
                    newLine[j1] = '>';
                    ++moveCount;
                }
            }
        }
        lines = newLines;

        // Move down
        for (size_t i = 0; i < HEIGHT; ++i)
        {
            std::string& line = lines[i];
            std::string& line1 = lines[(i + 1) % HEIGHT];
            std::string& newLine  = newLines[i];
            std::string& newLine1 = newLines[(i + 1) % HEIGHT];
            for (size_t j = 0; j < WIDTH; ++j)
            {
                if (line[j] == 'v' && line1[j] == '.')
                {
                    newLine[j] = '.';
                    newLine1[j] = 'v';
                    ++moveCount;
                }
            }
        }
        lines = newLines;
        ++iterations;
    } while (moveCount > 0);

    std::cout << "Count = " << iterations << std::endl;

    return 0;
}

static void readFile(char const * name, std::vector<std::string>& lines)
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
        lines.push_back(line);
    }
}

static void printMap(std::vector<std::string> const& map, int i)
{
    std::cout << "Iteration: " << i << std::endl;
    for (auto const& line : map)
        std::cout << line << std::endl;
}
