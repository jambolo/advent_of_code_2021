// Advent of Code 2021
// Day 24

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

#define TEST    1
#define PART_1  1

static char constexpr FILE_NAME[] = "day24-input.txt";

// z01 = (z00 % 26 + 10 != d01) ? (z00 /  1) * 26 + d01 + 13 : z00 /  1
// z02 = (z01 % 26 + 13 != d02) ? (z01 /  1) * 26 + d02 + 10 : z01 /  1
// z03 = (z02 % 26 + 13 != d03) ? (z02 /  1) * 26 + d03 +  3 : z02 /  1
// z04 = (z03 % 26 - 11 != d04) ? (z03 / 26) * 26 + d04 +  1 : z03 / 26
// z05 = (z04 % 26 + 11 != d05) ? (z04 /  1) * 26 + d05 +  9 : z04 /  1
// z06 = (z05 % 26 -  4 != d06) ? (z05 / 26) * 26 + d06 +  3 : z05 / 26
// z07 = (z06 % 26 + 12 != d07) ? (z06 /  1) * 26 + d07 +  5 : z06 /  1
// z08 = (z07 % 26 + 12 != d08) ? (z07 /  1) * 26 + d08 +  1 : z07 /  1
// z09 = (z08 % 26 + 15 != d09) ? (z08 /  1) * 26 + d09 +  0 : z08 /  1
// z10 = (z09 % 26 -  2 != d10) ? (z09 / 26) * 26 + d10 + 13 : z09 / 26
// z11 = (z10 % 26 -  5 != d11) ? (z10 / 26) * 26 + d11 +  7 : z10 / 26
// z12 = (z11 % 26 - 11 != d12) ? (z11 / 26) * 26 + d12 + 15 : z11 / 26
// z13 = (z12 % 26 - 13 != d13) ? (z12 / 26) * 26 + d13 + 12 : z12 / 26
// z14 = (z13 % 26 - 10 != d14) ? (z13 / 26) * 26 + d14 +  8 : z13 / 26


// z01 = d01 + 13
// z02 = (d01 + 13) * 26 + d02 + 10
// z03 = ((d01 + 13) * 26 + d02 + 10) * 26 + d03 +  3
// z04 = (d03 - 8 != d04) ? ((d01 + 13) * 26 + d02 + 10) * 26 + d04 +  1 : (d01 + 13) * 26 + d02 + 10
// z05 = (d03 - 8 != d04) ? (((d01 + 13) * 26 + d02 + 10) * 26 + d04 +  1) * 26 + d05 +  9 : ((d01 + 13) * 26 + d02 + 10) * 26 + d05 +  9
// z06 = (z05 % 26 -  4 != d06) ? (z05 / 26) * 26 + d06 +  3 : z05 / 26
// z07 = (z06 % 26 + 12 != d07) ? (z06 /  1) * 26 + d07 +  5 : z06 /  1
// z08 = (z07 % 26 + 12 != d08) ? (z07 /  1) * 26 + d08 +  1 : z07 /  1
// z09 = (z08 % 26 + 15 != d09) ? (z08 /  1) * 26 + d09 +  0 : z08 /  1
// z10 = (z09 % 26 -  2 != d10) ? (z09 / 26) * 26 + d10 + 13 : z09 / 26
// z11 = (z10 % 26 -  5 != d11) ? (z10 / 26) * 26 + d11 +  7 : z10 / 26
// z12 = (z11 % 26 - 11 != d12) ? (z11 / 26) * 26 + d12 + 15 : z11 / 26
// z13 = (z12 % 26 - 13 != d13) ? (z12 / 26) * 26 + d13 + 12 : z12 / 26
// z14 = (z13 % 26 - 10 != d14) ? (z13 / 26) * 26 + d14 +  8 : z13 / 26

// z14 = (2 != d14 - d13) ? (z12 / 26) * 26 + d14 +  8 : (z12 / 26)
// z14 = ((z12 / 26) % 26 - 10 != d14) ? (z13 / 26) * 26 + d14 +  8 : z13 / 26


static int constexpr NUMBER_OF_STAGES = 14;

class Alu
{
public:
    enum class Register
    {
        CONSTANT = -1,
        W = 0,
        X,
        Y,
        Z
    };

    enum class Operator
    {
        INP = 0,
        ADD,
        MUL,
        DIV,
        MOD,
        EQL
    };

    struct Instruction
    {
        Instruction(std::string const& line);
        Operator op;
        Register r0, r1;
        int constant;

        Operator parseOp(std::string const& s);
        Register parseRegister(std::string const& s);
    };

    Alu(std::vector<int> const& input);

    void reset(int64_t w, int64_t x, int64_t y, int64_t z)
    {
        registers_[0] = w;
        registers_[1] = x;
        registers_[2] = y;
        registers_[3] = z;
    }
    void reset(std::array<int64_t, 4> const& r) { registers_ = r; };
    void execute(std::vector<Instruction> const& instructions);

    int64_t w() const { return registers_[0]; }
    int64_t x() const { return registers_[1]; }
    int64_t y() const { return registers_[2]; }
    int64_t z() const { return registers_[3]; }

private:
    std::array<int64_t, 4> registers_ = { {0, 0, 0, 0} };
    std::vector<int> input_;
    std::vector<int>::iterator pInput_;
};

using Program = std::vector<Alu::Instruction>;

static void readFile(char const* name, std::vector<std::string> & lines);

int main(int argc, char** argv)
{
    // Read the input
    std::vector<std::string> lines;
    readFile(FILE_NAME, lines);

    Program program;
    for (auto const& line : lines)
    {
        Alu::Instruction instruction(line);
        program.emplace_back(instruction);
    }
    
    int64_t count = 0;
    while (true)
    {
        std::vector<int> input(14, 9);
        int64_t k = count;
        for (int i = 13; k > 0 && i >= 0; --i)
        {
            input[i] = 9 - k % 9;
            k = k / 9;
        }

        if (input[13] == 1 && input[12] == 1 && input[11] == 1 && input[10] == 1 && input[9] == 1 && input[8] == 1)
        {
            std::cerr << json(input) << std::endl;
        }

        Alu alu(input);
        alu.execute(program);
        if (alu.z() == 0)
        {
            std::cout << "Highest model number is " << json(input) << std::endl;
            break;
        }
        ++count;
    }
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
        if (line[0] != '#')
            lines.push_back(line);
    }
}

Alu::Instruction::Instruction(std::string const& line)
{
    std::regex  pattern("([a-z]+) ([w-z])( (([w-z])|(-?[0-9])))?");
    std::smatch match;
    bool found = std::regex_search(line, match, pattern);
    if (!found)
        throw std::runtime_error("regex failed");

    op = parseOp(match[1]);
    if (op == Operator::INP)
    {
        r0 = parseRegister(match[2]);
        r1 = Register::CONSTANT;
        constant = 0;
    }
    else
    {
        r0 = parseRegister(match[2]);
        if (match[6].matched)
        {
            r1 = Register::CONSTANT;
            constant = std::stoi(match[6]);
        }
        else
        {
            r1 = parseRegister(match[5]);
            constant = 0;
        }
    }
//    return
//    {
//        std::stoi(match[1].str()),
//        std::stoi(match[3].str()),
//        std::stoi(match[2].str()),
//        std::stoi(match[4].str())
//    };
}

Alu::Operator Alu::Instruction::parseOp(std::string const& s)
{
    if (s == "inp")
        return Operator::INP;
    else if (s == "add")
        return Operator::ADD;
    else if (s == "mul")
        return Operator::MUL;
    else if (s == "div")
        return Operator::DIV;
    else if (s == "mod")
        return Operator::MOD;
    else if (s == "eql")
        return Operator::EQL;
    throw std::runtime_error(std::string("Invalid operator: ") + s);
    return Operator();
}

Alu::Register Alu::Instruction::parseRegister(std::string const& s)
{
    return Register(s[0] - 'w');
}

Alu::Alu(std::vector<int> const& input)
    : input_(input)
    , pInput_(input_.begin())
{
}

void Alu::execute(std::vector<Instruction> const& instructions)
{
    for (auto const& i : instructions)
    {
        switch (i.op)
        {
        case Operator::INP:
            registers_[(int)i.r0] = *pInput_++;
            break;
        case Operator::ADD:
            if (i.r1 == Register::CONSTANT)
                registers_[(int)i.r0] += i.constant;
            else
                registers_[(int)i.r0] += registers_[(int)i.r1];
            break;
        case Operator::MUL:
            if (i.r1 == Register::CONSTANT)
                registers_[(int)i.r0] *= i.constant;
            else
                registers_[(int)i.r0] *= registers_[(int)i.r1];
            break;
        case Operator::DIV:
            if (i.r1 == Register::CONSTANT)
            {
                if (i.constant == 0)
                    throw std::runtime_error("Divide by 0");
                registers_[(int)i.r0] /= i.constant;
            }
            else
            {
                if (registers_[(int)i.r1] == 0)
                    throw std::runtime_error("Divide by 0");
                registers_[(int)i.r0] /= registers_[(int)i.r1];
            }
            break;
        case Operator::MOD:
            if (i.r1 == Register::CONSTANT)
            {
                if (i.constant <= 0)
                    throw std::runtime_error("Invalid mod divisor");
                registers_[(int)i.r0] %= i.constant;
            }
            else
            {
                if (registers_[(int)i.r1] <= 0)
                    throw std::runtime_error("Invalid mod divisor");
                registers_[(int)i.r0] %= registers_[(int)i.r1];
            }
            break;
        case Operator::EQL:
            if (i.r1 == Register::CONSTANT)
                registers_[(int)i.r0] = registers_[(int)i.r0] == i.constant;
            else
                registers_[(int)i.r0] = registers_[(int)i.r0] == registers_[(int)i.r1];
            break;
        }
    }
}
