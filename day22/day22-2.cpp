// Advent of Code 2021
// Day 22, part 2

#include "Cube.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
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

using CubeList = std::list<Cube>;

static void readFile(char const* name, std::vector<std::string>& lines);
static void add(CubeList& cubes, Cube const& x);
static void subtract(CubeList& cubes, Cube const& x);
static void coalesce(CubeList& cubes);
static CubeList difference(Cube const& a, Cube const& b);
static bool isDisjoint(CubeList& cubes);
static bool canBeCombined(Cube const& a, Cube const& b);
static Cube combine(Cube const& a, Cube const& b);

static CubeList on;

int main(int argc, char** argv)
{
    // Read the input
    std::vector<std::string> lines;
    readFile(FILE_NAME, lines);

    std::regex regex("(on|off) x=(-?[0-9]+)..(-?[0-9]+),y=(-?[0-9]+)..(-?[0-9]+),z=(-?[0-9]+)..(-?[0-9]+)");
    for (auto const& line : lines)
    {
        std::smatch match;
        if (std::regex_match(line, match, regex))
        {
            Cube cube(
                std::stoi(match[2].str()),
                std::stoi(match[3].str()),
                std::stoi(match[4].str()),
                std::stoi(match[5].str()),
                std::stoi(match[6].str()),
                std::stoi(match[7].str())
            );
            if (match[1].str() == "on")
                add(on, cube);
            else
                subtract(on, cube);
        }
    }

    int64_t count = 0;

    for (auto const& c : on)
    {
        count += c.size();
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

void add(CubeList& cubes, Cube const& x)
{
    CubeList added{ x };
    auto p0 = added.begin();
    while (p0 != added.end())
    {
        bool erased0 = false;
        auto p1 = cubes.begin();
        while (p1 != cubes.end())
        {
            bool erased1 = false;
            if (p0->intersects(*p1))
            {
                if (p1->contains(*p0))
                {
                    p0 = added.erase(p0);
                    erased0 = true;
                    break;
                }
                else if (p0->contains(*p1))
                {
                    p1 = cubes.erase(p1);
                    erased1 = true;
                }
                else
                {
                    CubeList split = difference(*p0, *p1);
                    added.splice(added.end(), split);
                    p0 = added.erase(p0);
                    erased0 = true;
                    break;
                }
            }
            if (!erased1)
                ++p1;
        }
        if (!erased0)
            ++p0;
    }
    cubes.splice(cubes.end(), added);
    if (cubes.size() > 1)
        coalesce(cubes);
}

void subtract(CubeList& cubes, Cube const& x)
{
    for (auto p1 = cubes.begin(); p1 != cubes.end(); )
    {
        if (x.intersects(*p1))
        {
            if (x.contains(*p1))
            {
                p1 = cubes.erase(p1);
            }
            else
            {
                CubeList remaining = difference(*p1, x);
                cubes.splice(cubes.end(), remaining);
                p1 = cubes.erase(p1);
            }
        }
        else
        {
            ++p1;
        }
    }
    if (cubes.size() > 1)
        coalesce(cubes);
}

static void coalesce(CubeList & cubes)
{
    assert(isDisjoint(cubes));
    for (auto p0 = std::next(cubes.begin()); p0 != cubes.end(); ++p0)
    {
        bool stillCombining = true;
        while (stillCombining)
        {
            stillCombining = false; // Assume nothing will be combined
            for (auto p1 = cubes.begin(); p1 != p0; )
            {
                if (canBeCombined(*p0, *p1))
                {
                    Cube combined = combine(*p0, *p1);
                    *p0 = combined;
                    p1 = cubes.erase(p1);
                    stillCombining = true;
                }
                else
                {
                    ++p1;
                }
            }
        }
    }
}


static CubeList difference(Cube const& a, Cube const& b)
{
    CubeList cubes;

    // 8 Corners
    if (a.intersects(Cube(b.minX_ - 1, b.minX_ - 1, b.minY_ - 1, b.minY_ - 1, b.minZ_ - 1, b.minZ_ - 1)))   // -x-y-z
    {
        cubes.emplace(
            cubes.end(),
            Cube(a.minX_, b.minX_ - 1,
                 a.minY_, b.minY_ - 1,
                 a.minZ_, b.minZ_ - 1)
        );
    }
    if (a.intersects(Cube(b.maxX_ + 1, b.maxX_ + 1, b.minY_ - 1, b.minY_ - 1, b.minZ_ - 1, b.minZ_ - 1)))   // +x-y-z
    {
        cubes.emplace(
            cubes.end(),
            Cube(b.maxX_ + 1, a.maxX_,
                 a.minY_, b.minY_ - 1,
                 a.minZ_, b.minZ_ - 1)
        );
    }
    if (a.intersects(Cube(b.minX_ - 1, b.minX_ - 1, b.maxY_ + 1, b.maxY_ + 1, b.minZ_ - 1, b.minZ_ - 1)))   // -x+y-z
    {
        cubes.emplace(
            cubes.end(),
            Cube(a.minX_, b.minX_ - 1,
                 b.maxY_ + 1, a.maxY_,
                 a.minZ_, b.minZ_ - 1)
        );
    }
    if (a.intersects(Cube(b.maxX_ + 1, b.maxX_ + 1, b.maxY_ + 1, b.maxY_ + 1, b.minZ_ - 1, b.minZ_ - 1)))   // +x+y-z
    {
        cubes.emplace(
            cubes.end(),
            Cube(b.maxX_ + 1, a.maxX_,
                 b.maxY_ + 1, a.maxY_,
                 a.minZ_, b.minZ_ - 1)
        );
    }
    if (a.intersects(Cube(b.minX_ - 1, b.minX_ - 1, b.minY_ - 1, b.minY_ - 1, b.maxZ_ + 1, b.maxZ_ + 1)))   // -x-y+z
    {
        cubes.emplace(
            cubes.end(),
            Cube(a.minX_, b.minX_ - 1,
                 a.minY_, b.minY_ - 1,
                 b.maxZ_ + 1, a.maxZ_)
        );
    }
    if (a.intersects(Cube(b.maxX_ + 1, b.maxX_ + 1, b.minY_ - 1, b.minY_ - 1, b.maxZ_ + 1, b.maxZ_ + 1)))   // +x-y+z
    {
        cubes.emplace(
            cubes.end(),
            Cube(b.maxX_ + 1, a.maxX_,
                 a.minY_, b.minY_ - 1,
                 b.maxZ_ + 1, a.maxZ_)
        );
    }
    if (a.intersects(Cube(b.minX_ - 1, b.minX_ - 1, b.maxY_ + 1, b.maxY_ + 1, b.maxZ_ + 1, b.maxZ_ + 1)))   // -x+y+z
    {
        cubes.emplace(
            cubes.end(),
            Cube(a.minX_, b.minX_ - 1,
                 b.maxY_ + 1, a.maxY_,
                 b.maxZ_ + 1, a.maxZ_)
        );
    }
    if (a.intersects(Cube(b.maxX_ + 1, b.maxX_ + 1, b.maxY_ + 1, b.maxY_ + 1, b.maxZ_ + 1, b.maxZ_ + 1)))   // +x+y+z
    {
        cubes.emplace(
            cubes.end(),
            Cube(b.maxX_ + 1, a.maxX_,
                 b.maxY_ + 1, a.maxY_,
                 b.maxZ_ + 1, a.maxZ_)
        );
    }

    // 12 Edges
    if (a.intersects(Cube(b.minX_ - 1, b.minX_ - 1, b.minY_, b.maxY_, b.minZ_ - 1, b.minZ_ - 1)))   // -x-z
    {
        cubes.emplace(
            cubes.end(),
            Cube(a.minX_, b.minX_ - 1,
                 std::max(a.minY_, b.minY_), std::min(a.maxY_, b.maxY_),
                 a.minZ_, b.minZ_ - 1)
        );
    }
    if (a.intersects(Cube(b.maxX_ + 1, b.maxX_ + 1, b.minY_, b.maxY_, b.minZ_ - 1, b.minZ_ - 1)))   // +x-z
    {
        cubes.emplace(
            cubes.end(),
            Cube(b.maxX_ + 1, a.maxX_,
                 std::max(a.minY_, b.minY_), std::min(a.maxY_, b.maxY_),
                 a.minZ_, b.minZ_ - 1)
        );
    }
    if (a.intersects(Cube(b.minX_, b.maxX_, b.minY_ - 1, b.minY_ - 1, b.minZ_ - 1, b.minZ_ - 1)))   // -y-z
    {
        cubes.emplace(
            cubes.end(),
            Cube(std::max(a.minX_, b.minX_), std::min(a.maxX_, b.maxX_),
                 a.minY_, b.minY_ - 1,
                 a.minZ_, b.minZ_ - 1)
        );
    }
    if (a.intersects(Cube(b.minX_, b.maxX_, b.maxY_ + 1, b.maxY_ + 1, b.minZ_ - 1, b.minZ_ - 1)))   // +y-z
    {
        cubes.emplace(
            cubes.end(),
            Cube(std::max(a.minX_, b.minX_), std::min(a.maxX_, b.maxX_),
                b.maxY_ + 1, a.maxY_,
                a.minZ_, b.minZ_ - 1)
        );
    }

    if (a.intersects(Cube(b.minX_ - 1, b.minX_ - 1, b.minY_, b.maxY_, b.maxZ_ + 1, b.maxZ_ + 1)))   // -x+z
    {
        cubes.emplace(
            cubes.end(),
            Cube(a.minX_, b.minX_ - 1,
                 std::max(a.minY_, b.minY_), std::min(a.maxY_, b.maxY_),
                 b.maxZ_ + 1, a.maxZ_)
        );
    }
    if (a.intersects(Cube(b.maxX_ + 1, b.maxX_ + 1, b.minY_, b.maxY_, b.maxZ_ + 1, b.maxZ_ + 1)))   // +x+z
    {
        cubes.emplace(
            cubes.end(),
            Cube(b.maxX_ + 1, a.maxX_,
                 std::max(a.minY_, b.minY_), std::min(a.maxY_, b.maxY_),
                 b.maxZ_ + 1, a.maxZ_)
        );
    }
    if (a.intersects(Cube(b.minX_, b.maxX_, b.minY_ - 1, b.minY_ - 1, b.maxZ_ + 1, b.maxZ_ + 1)))   // -y+z
    {
        cubes.emplace(
            cubes.end(),
            Cube(std::max(a.minX_, b.minX_), std::min(a.maxX_, b.maxX_),
                a.minY_, b.minY_ - 1,
                b.maxZ_ + 1, a.maxZ_)
        );
    }
    if (a.intersects(Cube(b.minX_, b.maxX_, b.maxY_ + 1, b.maxY_ + 1, b.maxZ_ + 1, b.maxZ_ + 1)))   // +y+z
    {
        cubes.emplace(
            cubes.end(),
            Cube(std::max(a.minX_, b.minX_), std::min(a.maxX_, b.maxX_),
                b.maxY_ + 1, a.maxY_,
                b.maxZ_ + 1, a.maxZ_)
        );
    }
    if (a.intersects(Cube(b.minX_ - 1, b.minX_ - 1, b.minY_ - 1, b.minY_ - 1, b.minZ_, b.maxZ_)))   // -x-y
    {
        cubes.emplace(
            cubes.end(),
            Cube(a.minX_, b.minX_ - 1,
                 a.minY_, b.minY_ - 1,
                 std::max(a.minZ_, b.minZ_), std::min(a.maxZ_, b.maxZ_))
        );
    }
    if (a.intersects(Cube(b.minX_ - 1, b.minX_ - 1, b.maxY_ + 1, b.maxY_ + 1, b.minZ_, b.maxZ_)))   // -x+y
    {
        cubes.emplace(
            cubes.end(),
            Cube(a.minX_, b.minX_ - 1,
                 b.maxY_ + 1, a.maxY_,
                 std::max(a.minZ_, b.minZ_), std::min(a.maxZ_, b.maxZ_))
        );
    }
    if (a.intersects(Cube(b.maxX_ + 1, b.maxX_ + 1, b.minY_ - 1, b.minY_ - 1, b.minZ_, b.maxZ_)))   // +x-y
    {
        cubes.emplace(
            cubes.end(),
            Cube(b.maxX_ + 1, a.maxX_,
                 a.minY_, b.minY_ - 1,
                 std::max(a.minZ_, b.minZ_), std::min(a.maxZ_, b.maxZ_))
        );
    }
    if (a.intersects(Cube(b.maxX_ + 1, b.maxX_ + 1, b.maxY_ + 1, b.maxY_ + 1, b.minZ_, b.maxZ_)))   // +x+y
    {
        cubes.emplace(
            cubes.end(),
            Cube(b.maxX_ + 1, a.maxX_,
                 b.maxY_ + 1, a.maxY_,
                 std::max(a.minZ_, b.minZ_), std::min(a.maxZ_, b.maxZ_))
        );
    }

    // 6 faces
    if (a.intersects(Cube(b.minX_ - 1, b.minX_ - 1, b.minY_, b.maxY_, b.minZ_, b.maxZ_)))   // -x
    {
        cubes.emplace(
            cubes.end(),
            Cube(a.minX_, b.minX_ - 1,
                 std::max(a.minY_, b.minY_), std::min(a.maxY_, b.maxY_),
                 std::max(a.minZ_, b.minZ_), std::min(a.maxZ_, b.maxZ_))
        );
    }
    if (a.intersects(Cube(b.maxX_ + 1, b.maxX_ + 1, b.minY_, b.maxY_, b.minZ_, b.maxZ_)))   // +x
    {
        cubes.emplace(
            cubes.end(),
            Cube(b.maxX_ + 1, a.maxX_,
                 std::max(a.minY_, b.minY_), std::min(a.maxY_, b.maxY_),
                 std::max(a.minZ_, b.minZ_), std::min(a.maxZ_, b.maxZ_))
        );
    }
    if (a.intersects(Cube(b.minX_, b.maxX_, b.minY_ - 1, b.minY_ - 1, b.minZ_, b.maxZ_)))   // -y
    {
        cubes.emplace(
            cubes.end(),
            Cube(std::max(a.minX_, b.minX_), std::min(a.maxX_, b.maxX_),
                 a.minY_, b.minY_ - 1,
                 std::max(a.minZ_, b.minZ_), std::min(a.maxZ_, b.maxZ_))
        );
    }
    if (a.intersects(Cube(b.minX_, b.maxX_, b.maxY_ + 1, b.maxY_ + 1, b.minZ_, b.maxZ_)))   // +y
    {
        cubes.emplace(
            cubes.end(),
            Cube(std::max(a.minX_, b.minX_), std::min(a.maxX_, b.maxX_),
                 b.maxY_ + 1, a.maxY_,
                 std::max(a.minZ_, b.minZ_), std::min(a.maxZ_, b.maxZ_))
        );
    }
    if (a.intersects(Cube(b.minX_, b.maxX_, b.minY_, b.maxY_, b.minZ_ - 1, b.minZ_ - 1)))   // -z
    {
        cubes.emplace(
            cubes.end(),
            Cube(std::max(a.minX_, b.minX_), std::min(a.maxX_, b.maxX_),
                 std::max(a.minY_, b.minY_), std::min(a.maxY_, b.maxY_),
                 a.minZ_, b.minZ_ - 1)
        );
    }
    if (a.intersects(Cube(b.minX_, b.maxX_, b.minY_, b.maxY_, b.maxZ_ + 1, b.maxZ_ + 1)))   // +z
    {
        cubes.emplace(
            cubes.end(),
            Cube(std::max(a.minX_, b.minX_), std::min(a.maxX_, b.maxX_),
                 std::max(a.minY_, b.minY_), std::min(a.maxY_, b.maxY_),
                 b.maxZ_ + 1, a.maxZ_)
        );
    }

    return cubes;
}

static bool isDisjoint(CubeList& cubes)
{
    for (auto p0 = cubes.begin(); p0 != std::prev(cubes.end()); ++p0)
    {
        for (auto p1 = std::next(p0); p1 != cubes.end(); ++p1)
        {
            if (intersects(*p0, *p1))
                return false;
        }
    }
    return true;
}

static bool canBeCombined(Cube const& a, Cube const& b)
{
    if (a.minX_ == b.minX_ && a.maxX_ == b.maxX_ && a.minY_ == b.minY_ && a.maxY_ == b.maxY_ && (a.minZ_ == b.maxZ_ + 1 || a.maxZ_ == b.minZ_ - 1))
        return true;
    if (a.minX_ == b.minX_ && a.maxX_ == b.maxX_ && a.minZ_ == b.minZ_ && a.maxZ_ == b.maxZ_ && (a.minY_ == b.maxY_ + 1 || a.maxY_ == b.minY_ - 1))
        return true;
    if (a.minY_ == b.minY_ && a.maxY_ == b.maxY_ && a.minZ_ == b.minZ_ && a.maxZ_ == b.maxZ_ && (a.minX_ == b.maxX_ + 1 || a.maxX_ == b.minX_ - 1))
        return true;
    return false;
}

static Cube combine(Cube const& a, Cube const& b)
{
    int minX = std::min(a.minX_, b.minX_);
    int maxX = std::max(a.maxX_, b.maxX_);
    int minY = std::min(a.minY_, b.minY_);
    int maxY = std::max(a.maxY_, b.maxY_);
    int minZ = std::min(a.minZ_, b.minZ_);
    int maxZ = std::max(a.maxZ_, b.maxZ_);

    return Cube(minX, maxX, minY, maxY, minZ, maxZ);
}
