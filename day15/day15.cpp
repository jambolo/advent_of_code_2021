// Advent of Code 2021
// Day 15

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "common/PathFinder.h"

using json = nlohmann::json;

static char constexpr FILE_NAME[] = "day15-input.txt";
static int constexpr REPEAT = 5;
static int constexpr WIDTH = 100 * REPEAT;
static int constexpr HEIGHT = 100 * REPEAT;
static int constexpr START = 0;
static int constexpr GOAL = WIDTH * HEIGHT - 1;

struct Day15Node : public PathFinder::Node
{
    float risk;
    int row;
    int column;

    Day15Node(char v, int r, int c) : risk(float(v)), row(r), column(c) {}
    virtual float h(Node const * goal) const override
    {
        Day15Node const* g = static_cast<Day15Node const*>(goal);
        return sqrt(float((row - g->row)*(row - g->row) + (column - g->column)*(column - g->column)));
    };
};

void to_json(json& j, PathFinder::Node const* p)
{
    Day15Node const * n = static_cast<Day15Node const *>(p);

    j["risk"] = n->risk;
    j["row"] = n->row;
    j["column"] = n->column;
}

void to_json(json& j, PathFinder::Edge const* e)
{
    j["cost"] = e->cost;
    j["to"] = e->to;
}

static void loadInput(char const* name, std::vector<std::string> & lines);

int main(int argc, char** argv)
{
    // Load the input
    std::vector<std::string> lines;
    loadInput(FILE_NAME, lines);

    // Create the nodes
    PathFinder::NodeList domain;
    for (int i = 0; i < REPEAT; ++i)
    {
        for (int r = 0; r < HEIGHT/REPEAT; ++r)
        {
            for (int j = 0; j < REPEAT; ++j)
            {
                std::string const& line = lines[r];
                for (int c = 0; c < WIDTH/REPEAT; ++c)
                {
                    char v = line[c];
                    domain.push_back(new Day15Node((v - '0' + i + j - 1) % 9 + 1, i * (HEIGHT / REPEAT) + r, j * (WIDTH / REPEAT) + c));
                }
            }
        }
    }

//    std::cerr << json(domain) << std::endl;

    // Create the edges
    PathFinder::EdgeList edges;
    for (int r = 0; r < HEIGHT; ++r)
    {
        for (int c = 0; c < WIDTH; ++c)
        {
            Day15Node* n0 = static_cast<Day15Node*>(domain[r * HEIGHT + c]);
            if (r > 0)
            {
                Day15Node* n1 = static_cast<Day15Node*>(domain[(r - 1) * HEIGHT + c]);
                n0->edges.push_back(new PathFinder::Edge{ n1->risk, n1 });
            }
            if (c > 0)
            {
                Day15Node* n1 = static_cast<Day15Node*>(domain[r * HEIGHT + c - 1]);
                n0->edges.push_back(new PathFinder::Edge{ n1->risk, n1 });
            }
            if (c < WIDTH - 1)
            {
                Day15Node* n1 = static_cast<Day15Node*>(domain[r * HEIGHT + c + 1]);
                n0->edges.push_back(new PathFinder::Edge{ n1->risk, n1 });
            }
            if (r < HEIGHT - 1)
            {
                Day15Node* n1 = static_cast<Day15Node*>(domain[(r + 1) * HEIGHT + c]);
                n0->edges.push_back(new PathFinder::Edge{ n1->risk, n1 });
            }
        }
    }

//    std::cerr << json(edges) << std::endl;

    // Find the lowest cost path from START to GOAL

    PathFinder pathfinder(&domain);
    PathFinder::Path path = pathfinder.findPath(domain[START], domain[GOAL]);

//    std::cerr << "Path Length: " << path.size() << std::endl;
//    std::cerr << json(path) << std::endl;

    float risk = 0.0f;
    for (auto p : path)
    {
        risk += static_cast<Day15Node*>(p)->risk;
    }
    risk -= static_cast<Day15Node*>(path[0])->risk; // Exclude the first cell

    std::cout << "Risk of lowest risk path = " << risk << std::endl;

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
        assert(line.size() == WIDTH/REPEAT);
        lines.emplace_back(line);
    }

    assert(lines.size() == HEIGHT/REPEAT);
}
