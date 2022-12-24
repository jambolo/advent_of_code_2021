// Advent of Code 2021
// Day 21, part 2


#include <array>
#include <iostream>
#include <vector>

//#define TEST    1

static int constexpr GOAL = 21;
static int constexpr MAX_TURNS = 7;
static int constexpr NUMBER_OF_POSITIONS = 10;
static std::array<int, 7> constexpr rollOutcomes = { 1, 3, 6, 7, 6, 3, 1 };

// Every possible state of the game at each turn
using State = std::array<std::array<std::array<std::array<int64_t, GOAL>, GOAL>, NUMBER_OF_POSITIONS>, NUMBER_OF_POSITIONS>;

static void clearState(State & state)
{
    for (int p1 = 0; p1 < NUMBER_OF_POSITIONS; ++p1)
        for (int p2 = 0; p2 < NUMBER_OF_POSITIONS; ++p2)
            for (int s1 = 0; s1 < GOAL; ++s1)
                for (int s2 = 0; s2 < GOAL; ++s2)
                    state[p1][p2][s1][s2] = 0;
}



State state;
State newState;

int main(int argc, char** argv)
{
    // Skip reading input, hardcode instead
#if defined(TEST)
    int position1 = 4 - 1;
    int position2 = 8 - 1;
#else
    int position1 = 4 - 1;
    int position2 = 3 - 1;
#endif

    state[position1][position2][0][0] = 1;

    int64_t wins1 = 0;
    int64_t wins2 = 0;
    int turn = 0;


    while (true)
    {
        // Player 1 moves
        int64_t remaining = 0;
        clearState(newState);

        for (int p1 = 0; p1 < NUMBER_OF_POSITIONS; ++p1)
        {
            for (int p2 = 0; p2 < NUMBER_OF_POSITIONS; ++p2)
            {
                for (int s1 = 0; s1 < GOAL; ++s1)
                {
                    for (int s2 = 0; s2 < GOAL; ++s2)
                    {
                        int64_t nUniverses = state[p1][p2][s1][s2];
                        if (nUniverses > 0)
                        {
                            for (int i = 0; i < rollOutcomes.size(); ++i)
                            {
                                int64_t nOutcomes = rollOutcomes[i];
                                int roll = i + 3;
                                int newPosition = (p1 + roll) % NUMBER_OF_POSITIONS;
                                int newScore = s1 + (newPosition + 1);
                                if (newScore >= GOAL)
                                {
                                    wins1 += nOutcomes * nUniverses;
                                }
                                else
                                {
                                    newState[newPosition][p2][newScore][s2] += nOutcomes * nUniverses;
                                    remaining += nOutcomes * nUniverses;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (remaining == 0)
            break;

#if TEST
        std::cerr << "Turn " << turn << ", remaining: " << remaining << std::endl;
#endif
        // Update state

        state = newState;

        // Player 2 moves
        remaining = 0;
        clearState(newState);

        for (int p1 = 0; p1 < NUMBER_OF_POSITIONS; ++p1)
        {
            for (int p2 = 0; p2 < NUMBER_OF_POSITIONS; ++p2)
            {
                for (int s1 = 0; s1 < GOAL; ++s1)
                {
                    for (int s2 = 0; s2 < GOAL; ++s2)
                    {
                        for (int i = 0; i < rollOutcomes.size(); ++i)
                        {
                            int64_t nUniverses = state[p1][p2][s1][s2];
                            if (nUniverses > 0)
                            {
                                int64_t nOutcomes = rollOutcomes[i];
                                int roll = i + 3;
                                int newPosition = (p2 + roll) % NUMBER_OF_POSITIONS;
                                int newScore = s2 + (newPosition + 1);
                                if (newScore >= GOAL)
                                {
                                    wins2 += nOutcomes * nUniverses;
                                }
                                else
                                {
                                    newState[p1][newPosition][s1][newScore] += nOutcomes * nUniverses;
                                    remaining += nOutcomes * nUniverses;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (remaining == 0)
            break;
#if TEST
        std::cerr << "Turn " << turn << ", remaining: " << remaining << std::endl;
#endif

        // Update state

        state = newState;

        ++turn;
    }

    std::cout << "Player 1 wins " << wins1 << " times. Player 2 wins " << wins2 << std::endl;
    return 0;
}
