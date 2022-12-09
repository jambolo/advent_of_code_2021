// Advent of Code 2021
// Day 3

#include <nlohmann/json.hpp>

#include <algorithm>
#include <bitset>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;

static size_t constexpr WIDTH = 12;
using Reading = std::bitset<WIDTH>;

static bool majority(std::vector<Reading> const & readings, int i);

int main(int argc, char ** argv)
{
    // Read the file

    std::ifstream input("day03-input.txt");
    if (!input.is_open())
        exit(1);

    std::vector<Reading> readings;

    while (true)
    {
        Reading reading;
        input >> reading;
        if (input.fail())
            break;

        readings.emplace_back(reading);
    }

    std::cout << "Number of readings = " << readings.size() << std::endl;

    // Compute gamma and epsilon and their product

    int gamma	= 0;	// More 1s than 0s
    int epsilon = 0;	// More 0s than 1s

    for (int i = 0; i < WIDTH; ++i)
    {
        if (majority(readings, i))
            gamma += 1 << i;
        else
            epsilon += 1 << i;
    }

    std::cout << "gamma = " << gamma << ", epsilon = " << epsilon << std::endl;
    std::cout << "product = " << gamma * epsilon << std::endl;

    // Compute O2 and CO2 and their product

    std::vector<Reading> o2Readings = readings;
    for (int i = WIDTH - 1; i >= 0; --i)
    {
        if (o2Readings.size() == 1)
            break;

        if (majority(o2Readings, i))
        {
            o2Readings.erase(
                std::remove_if(o2Readings.begin(), o2Readings.end(), [i](Reading const& r) { return !r[i]; }),
                o2Readings.end());
        }
        else
        {
            o2Readings.erase(
                std::remove_if(o2Readings.begin(), o2Readings.end(), [i](Reading const& r) { return r[i]; }),
                o2Readings.end());
        }
    }

    std::vector<Reading> co2Readings = readings;
    for (int i = WIDTH-1; i >= 0; --i)
    {
        if (co2Readings.size() == 1)
            break;

        if (majority(co2Readings, i))
        {
            co2Readings.erase(
                std::remove_if(co2Readings.begin(), co2Readings.end(), [i](Reading const& r) { return r[i]; }),
                co2Readings.end());
        }
        else
        {
            co2Readings.erase(
                std::remove_if(co2Readings.begin(), co2Readings.end(), [i](Reading const& r) { return !r[i]; }),
                co2Readings.end());
        }
    }

    std::cout << "O2 = " << o2Readings[0].to_ulong() << ", CO2 = " << co2Readings[0].to_ulong() << std::endl;
    std::cout << "product = " << o2Readings[0].to_ulong() * co2Readings[0].to_ulong() << std::endl;


    return 0;
}

static bool majority(std::vector<Reading> const& readings, int i)
{
    int count = 0;

    for (auto const& r : readings)
    {
        if (r[i])
            ++count;
    }

    return 2 * count >= readings.size();
}
