// Advent of Code 2021
// Day 20

#include "common/expect.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

//#define TEST    1
//#define PART_1  1

#if defined(TEST)
static char constexpr FILE_NAME[] = "day20-test.txt";
#else
static char constexpr FILE_NAME[] = "day20-input.txt";
#endif

#if defined(PART_1)
static int constexpr ITERATIONS = 2;
#else
static int constexpr ITERATIONS = 50;
#endif

static int constexpr BORDER = 3;

using Image = std::vector<std::vector<char>>;

static void readFile(char const* name, std::vector<std::string> & lines);
static void print(Image const& image);
static Image expand(Image const& image, int n);
static Image shrink(Image const& image, int n);
static Image map(Image const& image, std::string const & mapping);
static int mapPixel(Image const& image, std::string const& mapping, int r, int c);
static int64_t countPixels(Image const& image);

int main(int argc, char** argv)
{
    // Read the input
    std::vector<std::string> lines;
    readFile(FILE_NAME, lines);

    std::string mapping = lines[0];

#if defined(TEST)
    std::cerr << "Map size: " << mapping.size() << std::endl;
#endif

    Image image;
    for (int i = 2; i < lines.size(); ++i)
    {
        image.emplace_back(std::vector<char>(lines[i].begin(), lines[i].end()));
    }

#if defined(TEST)
    std::cerr << "Image size: " << image[0].size() << ", " << image.size() << std::endl;
    print(image);
#endif


#if defined(TEST)
    std::cerr << "Image size: " << image[0].size() << ", " << image.size() << std::endl;
    print(image);
#endif

    image = expand(image, BORDER * ITERATIONS);
    for (int i = 0; i < ITERATIONS; ++i)
    {
        image = map(image, mapping);
#if defined(TEST)
        std::cerr << "Iteration " << i+1 << std::endl;
        print(image);
#endif
    }
    image = shrink(image, BORDER * ITERATIONS / 2);
#if defined(TEST)
    std::cerr << "Result " << std::endl;
    print(image);
#endif
    int64_t count = countPixels(image);
    std::cout << "Number of lit pixels = " << count << std::endl;

    return 0;
}

static void readFile(char const * name, std::vector<std::string>& lines)
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

static void print(Image const& image)
{
#if defined(TEST)
    for (auto const & row : image)
    {
        std::cerr << std::string(row.begin(), row.end()) << std::endl;
    }
#else
    for (int i = 0; i < std::min(60, (int)image.size()); ++i)
    {
        auto const& row = image[i];
        std::cerr << std::string(&row[0], std::min(60, (int)row.size())) << std::endl;
    }
#endif
}

static Image expand(Image const& image, int n)
{
    Image newImage = image;
    std::vector<char> empty(image[0].size() + 2 * n, '.');

    // Expand the left side
    for (auto& row : newImage)
    {
        row.insert(row.begin(), n, '.');
        row.insert(row.end(), n, '.');
    }
    newImage.insert(newImage.begin(), n, empty);
    newImage.insert(newImage.end(), n, empty);

    return newImage;
}

static Image shrink(Image const& image, int n)
{
    Image newImage;
    newImage.reserve(image.size() - 2 * n);

    for (int i = n; i < image.size() - n; ++i)
    {
        newImage.emplace_back(std::vector<char>(image[i].begin() + n, image[i].end() - n));
    }

    return newImage;
}

static Image map(Image const& image, std::string const& mapping)
{
    size_t width = image[0].size();
    size_t height = image.size();
    std::vector<char> empty(width, '.');

    Image newImage(height);
    newImage.front() = empty;
    for (int r = 1; r < height - 1; ++r)
    {
        auto & row = newImage[r];
        row.reserve(width);
        row.push_back('.');
        for (int c = 1; c < width - 1; ++c)
        {
            row.push_back(mapPixel(image, mapping, r, c));
        }
        row.push_back('.');
    }
    newImage.back() = empty;

    return newImage;
}

static int mapPixel(Image const& image, std::string const& mapping, int r, int c)
{
    int index = 0;
    index = (index << 1) + (image[r - 1][c - 1] == '#');
    index = (index << 1) + (image[r - 1][c + 0] == '#');
    index = (index << 1) + (image[r - 1][c + 1] == '#');
    index = (index << 1) + (image[r + 0][c - 1] == '#');
    index = (index << 1) + (image[r + 0][c + 0] == '#');
    index = (index << 1) + (image[r + 0][c + 1] == '#');
    index = (index << 1) + (image[r + 1][c - 1] == '#');
    index = (index << 1) + (image[r + 1][c + 0] == '#');
    index = (index << 1) + (image[r + 1][c + 1] == '#');

    return mapping[index];
}

static int64_t countPixels(Image const& image)
{
    int64_t count = 0;
    for (auto const& row : image)
    {
        count += std::count(row.begin(), row.end(), '#');
    }

    return count;
}
