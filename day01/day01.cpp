// Advent of Code 2021
// Day 1

#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

static int constexpr WINDOW_SIZE = 3;

int computeWindow(std::vector<int>::const_iterator p)
{
	int window = 0;
	for (int i = 0; i < WINDOW_SIZE; ++i)
		window += *(p - i);
	return window;
}

int main(int argc, char ** argv)
{
	// Read the file
	std::ifstream input("day01-input.txt");
	if (!input.is_open())
		exit(1);
	
	std::vector<int> depths;
	while (true)
	{
		int depth;
		input >> depth;
		if (input.fail())
			break;
		depths.push_back(depth);
	}

	// Count the number of increasing depths
	int nIncreasingDepths = 0;
	int previousDepth = depths[0];
	for (auto i = depths.begin()+1; i != depths.end(); ++i)
	{
		int depth = *i;
		if (depth > previousDepth)
			++nIncreasingDepths;
		previousDepth = depth;
	}
	std::cout << "The number of increasing depths is " << nIncreasingDepths << std::endl;

	// Count the number of increasing windows
	int nIncreasingWindows = 0;
	int previousWindow = computeWindow(depths.begin() + WINDOW_SIZE - 1);
	for (auto i = depths.begin() + WINDOW_SIZE; i != depths.end(); ++i)
	{
		int window = computeWindow(i);
		if (window > previousWindow)
			++nIncreasingWindows;
		previousWindow = window;
	}
	std::cout << "The number of increasing windows is " << nIncreasingWindows << std::endl;	
}
