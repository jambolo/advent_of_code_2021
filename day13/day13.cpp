// Advent of Code 2021
// Day 13

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdint>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include "common/expect.h"

#define PART    1

using json = nlohmann::json;

static char constexpr FILE_NAME[] = "day13-input.txt";
static char constexpr DOT = '#';
struct Point
{
    int r, c;
};

void to_json(json& j, Point const& p)
{
    j = std::vector<int>{ p.c, p.r };
}
using Paper = std::vector<std::string>;

static void loadInput(char const* name, std::vector<std::string> & lines);
static void drawPaper(Paper const& paper);
static void fold(Paper& paper, Point const& p);
static int countDots(Paper const& paper);

static std::vector<Point> folds;
static Paper paper;

static int minC = 0;
static int minR = 0;
static int maxC = std::numeric_limits<int>::min();
static int maxR = std::numeric_limits<int>::min();
static int width, height;

int main(int argc, char** argv)
{
    static std::vector<Point> points;

    // Load the input
    std::vector<std::string> lines;
    loadInput(FILE_NAME, lines);

    // Load dots
    int i = 0;
    while (i < lines.size())
    {
        int r, c;

        if (lines[i].empty())
        {
            ++i;
            break;
        }
        std::istringstream lineStream(lines[i]);
        lineStream >> c >> expect(',') >> r;
        ++i;
        if (lineStream.fail())
            break;

//        minC = std::min(x, minC);
//        minR = std::min(y, minR);
        maxC = std::max(c, maxC);
        maxR = std::max(r, maxR);

        points.emplace_back(Point{ r, c });
    }

    // Load folds
    while (i < lines.size())
    {
        if (lines[i].empty())
        {
            ++i;
            break;
        }

        char direction;
        int location;

        // "fold along "
        std::istringstream lineStream(lines[i].substr(11));
        ++i;
        lineStream >> direction >> expect('=') >> location;
        if (lineStream.fail())
            break;

        if (direction == 'x')
            folds.push_back(Point{ 0, location });  // fold along column x
        else if (direction == 'y')
            folds.push_back(Point{ location, 0 });  // fold along row y
        else
            throw std::runtime_error("Invalid fold direction");
    }

    width = maxC - minC + 1;
    height = maxR - minR + 1;

    // Create the paper
    paper.reserve(height);
    for (int i = 0; i < height; ++i)
    {
        paper.push_back(std::string(width, '.'));
    }

    // Place the dots
    for (auto const& p : points)
        paper[p.r][p.c] = DOT;

//    drawPaper(paper);
//    std::cerr << json(folds) << std::endl;

    // Now fold
    for (auto const & f : folds)
        fold(paper, f);

    drawPaper(paper);

    std::cout << "Number of dots is " << countDots(paper) << std::endl;

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

static void drawPaper(Paper const& paper)
{
    for (auto const& row : paper)
    {
        std::cerr << row << std::endl;
    }
}

static void fold(Paper& paper, Point const& p)
{
    if (p.r == 0)
    {
//        std::cerr << "fold along x=" << p.c << std::endl;
        if (p.c < width / 2)
            throw std::runtime_error("fold is less than half");
        for (int c = 1; c < width - p.c; ++c)
        {
            for (int r = 0; r < height; ++r)
            {
                if (paper[r][p.c + c] == DOT)
                    paper[r][p.c - c] = DOT;
            }
        }
        for (auto & row : paper)
            row.resize(p.c);
        width = p.c;
    }
    else if (p.c == 0)
    {
//        std::cerr << "fold along y=" << p.r << std::endl;
        if (p.r < height / 2)
            throw std::runtime_error("fold is less than half");
        for (int r = 1; r < height - p.r; ++r)
        {
            for (int c = 0; c < width; ++c)
            {
                if (paper[p.r + r][c] == DOT)
                paper[p.r - r][c] = DOT;
            }
        }
        paper.resize(p.r);
        height = p.r;

    }
    else
        throw std::runtime_error("invalid fold");
}

static int countDots(Paper const& paper)
{
    int count = 0;
    for (auto const& row : paper)
        count += std::count(row.begin(), row.end(), DOT);
    return count;
}
