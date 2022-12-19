// Advent of Code 2021
// Day 17

#include <nlohmann/json.hpp>

#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#define PART 2

using json = nlohmann::json;

static char constexpr FILE_NAME[] = "day17-input.txt";

struct Target
{
    int x0, y0, x1, y1;
};
static void to_json(json& j, Target const& t)
{
    j = json::array({ json::array({ t.x0, t.y0 }), json::array({ t.x1, t.y1 }) });
}

static void loadInput(char const* name, std::vector<std::string> & lines);
static Target extractTarget(std::string const & line);
static void step(int& x, int& y, int& vX, int& vY);

int main(int argc, char** argv)
{
    // Load the input
    std::vector<std::string> lines;
    loadInput(FILE_NAME, lines);

    // Get the location and size of the target

    Target target = extractTarget(lines[0]);
//    std::cerr << "Target: " << json(target) << std::endl;

    // Compute the range of initial x velocities
    int minVx0 = 1;
    while (minVx0 * (minVx0 + 1) / 2 < target.x0)
        ++minVx0;
#if PART == 1
    int maxVx0 = minVx0;
    while (maxVx0 * (maxVx0 + 1) / 2 < target.x1)
        ++maxVx0;
#else
    int maxVx0 = target.x1;
#endif

//    std::cerr << "minVx0 = " << minVx0 << std::endl;
//    std::cerr << "maxVx0 = " << maxVx0 << std::endl;

    // Compute the range of initial y velocities
    int maxVy0 = std::max(std::max(abs(target.y0), abs(target.y1) ), abs(target.y1 - target.y0));
    int minVy0 = std::min(1, target.y0);

//    std::cerr << "minVy0 = " << minVy0 << std::endl;
//    std::cerr << "maxVy0 = " << maxVy0 << std::endl;

    // Do the simulation for all values of vX0 and save the highest y

    int maxY = target.y0;
    int count = 0;

    for (int vX0 = minVx0; vX0 <= maxVx0; ++vX0)
    {
        int vY0 = minVy0;

        while (vY0 <= maxVy0)
        {
            int vX = vX0;
            int vY = vY0;
            int x = 0;
            int y = 0;
            int peak = (vY >= 0) ? (vY * (vY + 1 )) / 2 : 0;
//            std::cerr << "[" << vX0 << ", " << vY0 << "] ";
            while (true)
            {
                if (x >= target.x0 && x <= target.x1 && y >= target.y0 && y <= target.y1)
                {
                    // Hit
//                    std::cerr << "Hit: [" << x << ", " << y << "], peak is " << peak << std::endl;
                    maxY = std::max(maxY, peak);
                    ++count;
                    break;
                }
                else if (y < target.y0 || x > target.x1)
                {
                    // Miss
//                    std::cerr << "Miss" << std::endl;
                    break;
                }
                step(x, y, vX, vY);
            }
            ++vY0;
        }
    }

    std::cout << "Highest peak: " << maxY << std::endl;
    std::cout << "Number of successful tries: " << count << std::endl;

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

static Target extractTarget(std::string const& line)
{
    std::regex pattern("target area: x=(-?[0-9]+)..(-?[0-9]+), y=(-?[0-9]+)..(-?[0-9]+)");
    std::smatch match;
    bool found = std::regex_match(line, match, pattern);
    if (!found)
        throw std::runtime_error("regex failed");

    return
    {
        std::stoi(match[1].str()),
        std::stoi(match[3].str()),
        std::stoi(match[2].str()),
        std::stoi(match[4].str())
    };
}

static void step(int& x, int& y, int& vX, int& vY)
{
    x += vX;
    y += vY;
    if (vX > 0)
        --vX;
    --vY;
}