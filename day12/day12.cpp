// Advent of Code 2021
// Day 12

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

static char constexpr FILE_NAME[] = "day12-input.txt";

struct Node
{
    std::string name;
    bool big;
    std::vector<std::weak_ptr<Node>> edges;
};


static void loadInput(char const* name, std::vector<std::string> & lines);
static std::shared_ptr<Node> createNode(std::string const& name);
static void findPathToEnd(std::shared_ptr<Node> node, std::vector<std::string>& path, std::vector< std::vector<std::string>>& paths, bool twice);

void to_json(json& j, Node const & n)
{
    j["name"] = n.name;
    j["big"] = n.big;
    std::vector<std::string> edges;
    for (auto e : n.edges)
    {
        auto s = e.lock();
        if (s)
            edges.push_back(s->name);
    }
    j["edges"] = edges;
}

// void to_json(json& j, std::weak_ptr<Node> p)
// {
//     auto s = p.lock();
//     if (s)
//         j = s->name;
//     else
//         j = nullptr;
// }
// 
void to_json(json& j, std::shared_ptr<Node> p)
{
    if (p)
        j = *p;
    else
        j = nullptr;
}

std::vector<std::shared_ptr<Node>> nodes;
std::map<std::string, std::weak_ptr<Node>> nodeMap;

int main(int argc, char** argv)
{
    // Load the input
    std::vector<std::string> lines;
    loadInput(FILE_NAME, lines);

    // Build the graph

    for (auto const& line : lines)
    {
        size_t i = line.find('-');
        if (i == line.npos)
            throw std::runtime_error("missing dash");
        std::string name0 = line.substr(0, i);
        std::string name1 = line.substr(i + 1);

        auto i0 = nodeMap.find(name0);
        std::shared_ptr<Node> node0;
        if (i0 == nodeMap.end())
            node0 = createNode(name0);
        else
            node0 = i0->second.lock();

        auto i1 = nodeMap.find(name1);
        std::shared_ptr<Node> node1;
        if (i1 == nodeMap.end())
            node1 = createNode(name1);
        else
            node1 = i1->second.lock();

        node0->edges.push_back(node1);
        node1->edges.push_back(node0);
    }

//    std::cerr << json(nodes) << std::endl;

    // Depth first span of the graph from start to end

    std::vector<std::string> path;
    std::vector< std::vector<std::string>> paths;

    std::shared_ptr<Node> start = nodeMap["start"].lock();
    findPathToEnd(start, path, paths, false);

//    for (auto const& p : paths)
//        std::cerr << json(p) << std::endl;

    std::cout << paths.size() << " distinct paths found" << std::endl;

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

static std::shared_ptr<Node> createNode(std::string const& name)
{
    std::shared_ptr<Node> node(new Node{ name, std::isupper(name[0]) != 0 });
    nodes.push_back(node);
    nodeMap.insert(std::pair<std::string, std::weak_ptr<Node>>(name, node));
    return node;
}

static void findPathToEnd(std::shared_ptr<Node> node, std::vector<std::string>& path, std::vector< std::vector<std::string>>& paths, bool twice)
{
    path.push_back(node->name);
    if (node->name == "end")
    {
        paths.push_back(path);
    }
    else
    {
        for (auto e : node->edges)
        {
            auto s = e.lock();
            if (s->name != "start")
            {
                if (s->big)
                {
                    findPathToEnd(s, path, paths, twice);
                }
                else
                {
                    int count = (int)std::count(path.begin(), path.end(), s->name);
                    if (count < 1)
                    {
                        findPathToEnd(s, path, paths, twice);
                    }
                    else if (!twice && count < 2)
                    {
                        findPathToEnd(s, path, paths, true);
                    }
                }
            }
        }
    }
    path.pop_back();
}
