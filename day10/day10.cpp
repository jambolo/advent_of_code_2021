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

#define PART1 1

using json = nlohmann::json;

static char constexpr FILE_NAME[] = "day10-input.txt";

std::map<char, int64_t> corruptionPoints{ { ')', 3 }, { ']', 57 }, { '}', 1197 }, { '>', 25137 } };
std::map<char, int64_t> completionPoints{ { '(', 1 }, { '[', 2 }, { '{', 3 }, { '<', 4 } };

static void loadInput(char const* name, std::vector<std::string> & lines);
static int64_t checkCorruption(std::string const& line, std::vector<char>& scopes);
static int64_t checkCompletion(std::vector<char>& scopes);

int main(int argc, char** argv)
{
    std::vector<std::string> lines;
    // Load the input
    loadInput(FILE_NAME, lines);

//    std::cerr << "Input: " << json(lines) << std::endl;

    int64_t corruptionScore = 0;
    std::vector<int64_t> completionScores;
    std::vector<char> scopes;

    for (auto const& line : lines)
    {
        scopes.clear();
        int64_t corruption = checkCorruption(line, scopes);
        corruptionScore += corruption;
        if (corruption == 0)
        {
            int64_t completion = checkCompletion(scopes);
            completionScores.push_back(completion);
        }
    }
    std::cout << "Corruption score is " << corruptionScore << std::endl;

    std::sort(completionScores.begin(), completionScores.end());
    std::cout << "Completion score is " << completionScores[completionScores.size() / 2] << std::endl;

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

static int64_t checkCorruption(std::string const & line, std::vector<char> & scopes)
{
    int64_t score = 0;
    bool error = false;

    for (char c : line)
    {
        switch (c)
        {
        case '(':
        case '[':
        case '{':
        case '<':
            scopes.push_back(c);
            break;
        case ')':
            if (scopes.back() != '(')
            {
                score += corruptionPoints[c];
                error = true;
            }
            else
            {
                scopes.pop_back();
            }
            break;
        case ']':
            if (scopes.back() != '[')
            {
                score += corruptionPoints[c];
                error = true;
            }
            else
            {
                scopes.pop_back();
            }
            break;
        case '}':
            if (scopes.back() != '{')
            {
                score += corruptionPoints[c];
                error = true;
            }
            else
            {
                scopes.pop_back();
            }
            break;
        case '>':
            if (scopes.back() != '<')
            {
                score += corruptionPoints[c];
                error = true;
            }
            else
            {
                scopes.pop_back();
            }
            break;
        default:
            throw std::runtime_error(std::string("Invalid input character: '") + c + "'.");
        }
        if (error)
            break;
    }
    return score;
}

static int64_t checkCompletion(std::vector<char>& scopes)
{
    int64_t score = 0;
    for (auto p = scopes.rbegin(); p != scopes.rend(); ++p)
    {
        score = score * 5 + completionPoints[*p];
    }

    return score;
}