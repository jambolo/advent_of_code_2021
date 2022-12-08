// Advent of Code 2021
// Day 2

#include <nlohmann/json.hpp>

#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

using json = nlohmann::json;


struct Move
{
	std::string direction;
	int distance;
};

void to_json(json & j, Move const & m)
{
	j = json{ {"direction", m.direction}, {"distance", m.distance} };
}

void from_json(json const & j, Move & m)
{
	j.at("direction").get_to(m.direction);
	j.at("distance").get_to(m.distance);
}

int main(int argc, char ** argv)
{
	// Read the file
	std::ifstream input("day02-input.txt");
	if (!input.is_open())
		exit(1);

	std::vector <Move> moves;

	while (true)
	{
		std::string direction;
		int distance;
		input >> direction >> distance;
		if (input.fail())
			break;
		moves.push_back({ direction, distance });
	}

//	json j = moves;
//	std::cout << j << std::endl;

	// Compute the end point

	int distance = 0;
	int depth = 0;
	int aim = 0;
	int aimedDepth = 0;

	for (auto const& m : moves)
	{
		if (m.direction == "forward")
		{
			distance += m.distance;
			aimedDepth += aim * m.distance;
		}
		else if (m.direction == "up")
		{
			depth -= m.distance;
			aim -= m.distance;
		}
		else if (m.direction == "down")
		{
			depth += m.distance;
			aim += m.distance;
		}
		else
			throw std::runtime_error("Invalid direction: " + m.direction);
	}

	std::cout << "Distance is " << distance << ", depth is " << depth << std::endl;
	std::cout << "Product is " << distance * depth << std::endl;

	std::cout << "Aimed distance is " << distance << ", depth is " << aimedDepth << std::endl;
	std::cout << "Aimed product is " << distance * aimedDepth << std::endl;

	return 0;
}
