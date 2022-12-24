// Advent of Code 2021
// Day 22, part 1

#include "Cube.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

//#define TEST    1

#if defined(TEST)
static char constexpr FILE_NAME[] = "day22-test.txt";
#else
static char constexpr FILE_NAME[] = "day22-input.txt";
#endif


struct Step
{
    bool on;
    Cube region;
};

void to_json(json& j, Step const& s);

static void readFile(char const* name, std::vector<std::string>& lines);

std::array<std::array<std::array<bool, 101>, 101>, 101> reactor;

int main(int argc, char** argv)
{
    // Read the input
    std::vector<std::string> lines;
    readFile(FILE_NAME, lines);

    // Parse the lines
    std::vector<Step> steps;
    Cube bounds(-50, 50, -50, 50, -50, 50);

    std::regex regex("(on|off) x=(-?[0-9]+)..(-?[0-9]+),y=(-?[0-9]+)..(-?[0-9]+),z=(-?[0-9]+)..(-?[0-9]+)");
    for (auto const& line : lines)
    {
        std::smatch match;
        if (std::regex_match(line, match, regex))
        {
            Step step{
                match[1].str() == "on",
                Cube(
                std::stoi(match[2].str()),
                std::stoi(match[3].str()),
                std::stoi(match[4].str()),
                std::stoi(match[5].str()), 
                std::stoi(match[6].str()),
                std::stoi(match[7].str())
                )
            };
            if (bounds.intersects(step.region))
                steps.push_back(step);
        }
    }

    // Initialize the reactor
    for (auto const& step : steps)
    {
        for (int x = step.region.minX_; x <= step.region.maxX_; ++x)
        {
            for (int y = step.region.minY_; y <= step.region.maxY_; ++y)
            {
                for (int z = step.region.minZ_; z <= step.region.maxZ_; ++z)
                {
                    reactor[x+50][y+50][z+50] = step.on;
                }
            }
        }
    }

    // Count the cubes that are on
    int count = 0;
    for (auto const& x : reactor)
    {
        for (auto const& y : x)
        {
            for (auto z : y)
            {
                if (z)
                    ++count;
            }
        }
    }

    std::cout << "Number of ON cubes: " << count << std::endl;
    return 0;
}

static void readFile(char const* name, std::vector<std::string>& lines)
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

void to_json(json& j, Step const& s)
{
    j["on"] = s.on;
    j["region"] = s.region;
}
