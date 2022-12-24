// Advent of Code 2021
// Day 21, part 1

#include <iostream>

#define TEST    1
//#define PART_2  1

int constexpr GOAL = 1000;

static int roll();
static int move(int position, int roll);

static int rollCount = 0;

int main(int argc, char** argv)
{
    // Skip reading input, hardcode instead
#if defined(TEST)
    int position1 = 4;
    int position2 = 8;
#else
    int position1 = 4;
    int position2 = 3;
#endif

    int score1 = 0;
    int score2 = 0;

    while (true)
    {
        position1 = move(position1, roll() + roll() + roll());
        score1 += position1;
        if (score1 >= GOAL)
            break;

        position2 = move(position2, roll() + roll() + roll());
        score2 += position2;
        if (score2 >= GOAL)
            break;
    }

    int loserScore = std::min(score1, score2);

    std::cout << "Result: " << loserScore * rollCount << ", loser score: " << loserScore << ", rollCount: " << rollCount << std::endl;

    return 0;
}

static int roll()
{
    static int i = 1;
    ++rollCount;
    return i++;
}

static int move(int position, int roll)
{
    position += roll;
    position = (position - 1) % 10 + 1; // positions are number 1 - 10
    return position;
}
