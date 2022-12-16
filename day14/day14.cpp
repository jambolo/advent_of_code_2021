// Advent of Code 2021
// Day 14

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

#include "common/expect.h"

#define PART    1

using json = nlohmann::json;

static char constexpr FILE_NAME[] = "day14-input.txt";
static int constexpr NUMBER_OF_STEPS = 40;

struct Pair
{
    int insert;    // Id of inserted symbol
    int result[2]; // New pairs created by this rule
};

void to_json(json & j, Pair const& pair)
{
    j["insert"] = pair.insert;
    j["result"] = json::array({ pair.result[0], pair.result[1] });
}

using SymbolMap = std::map<char, int>;
using Rules = std::map<std::pair<char, char>, char>;

static void loadInput(char const* name, std::vector<std::string> & lines);
static void registerSymbol(char c);
static std::vector<int64_t> doInsertions();

static std::vector<Pair> pairs;             // by pair id
static SymbolMap symbols;                   // Maps character to id
static std::vector<int64_t> symbolCounts;   // by symbol id
static std::vector<int64_t> pairCounts;     // By pair id

static int pairId(int s0, int s1)
{
    return s0 * (int)symbols.size() + s1;
}

static int symbolId(char s)
{
    return symbols[s];
}

int main(int argc, char** argv)
{
    // Load the input
    std::vector<std::string> lines;
    loadInput(FILE_NAME, lines);

    // Get the template
    std::string start = lines[0];

    Rules rules;
    for (int i = 2; i < lines.size(); ++i)
    {
        std::string const& line = lines[i];
        rules[std::pair<char, char>(line[0], line[1])] = line[6];
    }

    // Register all of the symbols
    for (char c : start)
        registerSymbol(c);
    for (auto const& r : rules)
    {
        registerSymbol(r.first.first);
        registerSymbol(r.first.second);
        registerSymbol(r.second);
    }
    pairs.resize(symbols.size() * symbols.size());  // Create room for every possible pair
    pairCounts.resize(symbols.size() * symbols.size(), 0);
    symbolCounts.resize(symbols.size(), 0);

    // Create the pairs from the rules
    for (auto const& r : rules)
    {
        char left = r.first.first;
        char right = r.first.second;
        char inserted = r.second;
        int sLeft = symbolId(left);
        int sRight = symbolId(right);
        int sInserted = symbolId(inserted);
        int p = pairId(sLeft, sRight);
        pairs[p] = Pair{ sInserted, { pairId(sLeft, sInserted), pairId(sInserted, sRight) } };
    }

    // Process the starting template
    for (int i = 0; i < start.size() - 1; ++i)
    {
        char c0 = start[i];
        char c1 = start[i + 1];
        int s0 = symbolId(c0);
        int s1 = symbolId(c1);
        ++symbolCounts[s0];
        ++pairCounts[pairId(s0, s1)];

    }
    ++symbolCounts[symbolId(start.back())]; // Don't forget the last symbol

    for (auto const& s : symbols)
//        std::cerr << "  '" << char(s.first) << "': " << s.second;
//    std::cerr << std::endl;
//    std::cerr << "Symbols: " << json(symbols) << std::endl;
//    std::cerr << "Symbol Counts (0): " << json(symbolCounts) << std::endl;
//    std::cerr << "Pairs: " << json(pairCounts) << std::endl;

    for (int i = 0; i < NUMBER_OF_STEPS; ++i)
    {
        pairCounts = doInsertions();
//        std::cerr << "Symbol Counts (" << i + 1 << "): " << json(symbolCounts) << std::endl;
//        std::cerr << "Pairs: " << json(pairCounts) << std::endl;
    }

    std::vector<int64_t> sorted_counts = symbolCounts;
    std::sort(sorted_counts.begin(), sorted_counts.end());

    int64_t result = sorted_counts.back() - sorted_counts.front();
    std::cout << "Difference between highest and lowest count = " << result << std::endl;

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

static void registerSymbol(char c)
{
    static int symbolId = 0;

    auto it = symbols.find(c);
    if (it == symbols.end())
        symbols[c] = symbolId++;
}

static std::vector<int64_t> doInsertions()
{
    std::vector<int64_t> new_pairCounts(pairCounts.size(), 0);
    
    // Reset the pair counts from th

    for (int i = 0; i < pairs.size(); ++i)
    {
        auto const& p = pairs[i];
        int64_t count = pairCounts[i];
        symbolCounts[p.insert] += count;
        new_pairCounts[p.result[0]] += count;
        new_pairCounts[p.result[1]] += count;
    }
    return new_pairCounts;
}