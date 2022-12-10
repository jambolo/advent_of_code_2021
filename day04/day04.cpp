// Advent of Code 2021
// Day 4

#include <nlohmann/json.hpp>

#include <algorithm>
#include <bitset>
#include <cctype>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using json = nlohmann::json;

using Board = std::array<std::array<int, 5>, 5>;

static void readPicks(std::ifstream& input, std::vector<int>& picks);
static void loadBoard(std::ifstream& input, Board & board);
static bool playBoard(Board & board, int pick);

struct extract
{
    char c;
    extract(char c) : c(c) { }
};

std::istream& operator >> (std::istream& in, extract e)
{
    // Skip leading whitespace IFF user is not asking to extract a whitespace character
    if (!std::isspace(e.c))
        in >> std::ws;

    // Attempt to get the specific character
    if (in.peek() == e.c)
        in.get();
    else
        in.setstate(std::ios::failbit);

    return in;
}

int main(int argc, char ** argv)
{
    std::string empty; // for skipping lines

    // Read the file

    std::ifstream input("day04-input.txt");
    if (!input.is_open())
        exit(1);

    // Load picked numbers

    std::vector<int> picks;
    readPicks(input, picks);
    std::getline(input, empty);

    // Load boards

    std::vector<Board> boards;
    do
    {
        Board board;
        loadBoard(input, board);
        if (input.fail())
            break;
        boards.emplace_back(board);

    } while (!input.eof());

    //std::cerr << json(picks) << std::endl;
    //for (auto const & board : boards)
    //{
    //    std::cerr << json(board) << std::endl;
    //}

    // Play the game

    bool bingo = false;
    for (int pick : picks)
    {
        auto pB = boards.begin();
        while (pB != boards.end())
        {
            Board& board = *pB;

            bingo = playBoard(board, pick);
            if (bingo)
            {
                int unmarkedSum = 0;
                // Score it
                for (int i = 0; i < 5; ++i)
                {
                    for (int j = 0; j < 5; ++j)
                    {
                        if (board[i][j] >= 0)
                            unmarkedSum += board[i][j];
                    }
                }

                // Print the results
                std::cerr << "Winning pick is " << pick << std::endl;
                std::cerr << "Unmarked sum is " << unmarkedSum << std::endl;
                std::cerr << "Product is " << pick * unmarkedSum << std::endl;

                //for (auto const& board : boards)
                //{
                //    std::cerr << json(board) << std::endl;
                //}

                pB = boards.erase(pB);
            }
            else
            {
                ++pB;
            }
        }

    }
}

static void readPicks(std::ifstream& input, std::vector<int>& picks)
{
    std::string line;
    std::getline(input, line);
    if (input.fail())
        return;

    std::istringstream lineStream(line);
    while (!lineStream.fail())
    {
        int pick;
        lineStream >> pick;
        if (!lineStream.fail())
            picks.emplace_back(pick);
        lineStream >> extract(',');
    }
}

void loadBoard(std::ifstream& input, Board & board)
{
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            input >> board[i][j];
            if (input.fail())
                break;
            if (board[i][j] < 0)
                throw std::runtime_error("Board has negative cell.");
        }
    }
}

static bool playBoard(Board & board, int pick)
{
    // Mark a pick if it is found on the board.
    bool found = false;
    for (int r = 0; r < 5; ++r)
    {
        for (int c = 0; c < 5; ++c)
        {
            if (board[r][c] == pick)
            {
                board[r][c] = -1;       // Mark it as found
                found = true;
            }
        }
    }

    // If the pick was found, check for bingo
    if (found)
    {
        // Check the rows
        for (int r = 0; r < 5; ++r)
        {
            if (board[r][0] < 0 && board[r][1] < 0 && board[r][2] < 0 && board[r][3] < 0 && board[r][4] < 0)
            {
                return true;
            }
        }

        // Check the columns
        for (int c = 0; c < 5; ++c)
        {
            if (board[0][c] < 0 && board[1][c] < 0 && board[2][c] < 0 && board[3][c] < 0 && board[4][c] < 0)
            {
                return true;
            }
        }
    }

    return false;
}