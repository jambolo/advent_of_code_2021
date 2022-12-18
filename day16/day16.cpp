// Advent of Code 2021
// Day 16

#include <nlohmann/json.hpp>

#include <any>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;

static char constexpr FILE_NAME[] = "day16-input.txt";

enum PacketType
{
    OPERATOR_SUM        = 0,
    OPERATOR_PRODUCT    = 1,
    OPERATOR_MIN        = 2,
    OPERATOR_MAX        = 3,
    LITERAL             = 4,
    OPERATOR_GREATER    = 5,
    OPERATOR_LESS       = 6,
    OPERATOR_EQUAL      = 7
};
enum SubPacketListSizeType
{
    SUB_PACKET_LENGTH = 0,
    SUB_PACKET_COUNT  = 1
};

static size_t constexpr PACKET_VERSION_SIZE = 3;
static size_t constexpr PACKET_TYPE_SIZE = 3;
static size_t constexpr LITERAL_GROUP_SIZE = 5;
static size_t constexpr SUB_PACKET_LENGTH_SIZE = 15;
static size_t constexpr SUB_PACKET_COUNT_SIZE  = 11;

struct Packet;
using PacketList = std::vector<Packet>;

struct Packet
{
    int version;
    int type;
    std::any value;

    bool hasValue() const { return value.has_value(); }
    bool containsLiteral() const { return value.type() == typeid(int64_t); }
    bool containsPackets() const { return value.type() == typeid(PacketList); }
    int64_t evaluate() const;
};

static void to_json(json& j, Packet const& packet)
{
    j["version"] = packet.version;
    j["type"] = packet.type;
    if (packet.hasValue())
    {
        if (packet.containsLiteral())
            j["value"] = std::any_cast<int64_t>(packet.value);
        else if (packet.containsPackets())
            j["value"] = std::any_cast<PacketList const &>(packet.value);
        else
            j["value"] = nullptr;
    }
    else
    {
        j["value"] = nullptr;
    }
}

static void loadInput(char const* name, std::vector<std::string> & lines);
static int xtoi(char c);
static std::string convertToBinary(std::string const& hex);
static size_t parseInt(int64_t& value, std::string const& binary, size_t start, size_t end);
static size_t parseLiteral(int64_t& value, std::string const& binary, size_t start, size_t end);
static size_t parsePacketList(PacketList& packets, std::string const& binary, size_t start, size_t end);
static size_t parsePacketList(PacketList& packets, size_t count, std::string const& binary, size_t start, size_t end);
static size_t parsePacket(Packet& packet, std::string const& binary, size_t start, size_t end);
static int64_t sumOfVersionNumbers(Packet const & packet);

int main(int argc, char** argv)
{
    // Load the input
    std::vector<std::string> lines;
    loadInput(FILE_NAME, lines);

    for (auto const& line : lines)
    {
        std::cerr << "hex   : " << line << std::endl;
        std::string binary = convertToBinary(line);
//        std::cerr << "binary: " << binary << std::endl;

        size_t start = 0;
        Packet packet;

        start = parsePacket(packet, binary, start, binary.size());
//        std::cerr << json(packet).dump(2) << std::endl;

        int64_t sum = sumOfVersionNumbers(packet);
        std::cout << "Sum of version numbers = " << sum << std::endl;

        std::cout << "Packet evaluates to " << packet.evaluate() << std::endl;
    }

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


static int xtoi(char c) { return (c <= '9') ? c - '0' : ((c <= 'F') ? c - 'A' : c - 'a') + 10; }

static std::string convertToBinary(std::string const& hex)
{

    static char constexpr digits[16][5] =
    {
        "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
    };

    std::string binary;
    binary.reserve(4 * hex.size());

    for (char h : hex)
    {
        int b = xtoi(h);
        binary.insert(binary.size(), digits[b]);
    }
    return binary;
}

static size_t parseInt(int64_t& value, std::string const& binary, size_t start, size_t end)
{
    size_t length;
    long long v = std::stoll(binary.substr(start, end - start), &length, 2);
    value = v;
    return start + length;
}

static size_t parseLiteral(int64_t & value, std::string const& binary, size_t start, size_t end)
{
    std::string literal;
    char marker;
    do
    {
        marker = binary[start];
        literal.insert(literal.size(), binary, start + 1, LITERAL_GROUP_SIZE - 1);
        start += LITERAL_GROUP_SIZE;
    } while (marker == '1');
    value = (int64_t)std::stoll(literal, nullptr, 2);
    return start;
}

static size_t parsePacketList(PacketList& packets, std::string const& binary, size_t start, size_t end)
{
    while (start < end)
    {
        Packet packet;
        start = parsePacket(packet, binary, start, end);
        packets.emplace_back(packet);
    }
    return start;
}

static size_t parsePacketList(PacketList& packets, size_t count, std::string const& binary, size_t start, size_t end)
{
    for (size_t i = 0; i < count; ++i)
    {
        Packet packet;
        start = parsePacket(packet, binary, start, end);
        packets.emplace_back(packet);
    }
    return start;
}

static size_t parsePacket(Packet& packet, std::string const& binary, size_t start, size_t end)
{
    int64_t version;
    start = parseInt(version, binary, start, start + PACKET_VERSION_SIZE);
    packet.version = (int)version;

    int64_t type;
    start = parseInt(type, binary, start, start + PACKET_TYPE_SIZE);
    packet.type = (int)type;

    if (packet.type == LITERAL)
    {
        int64_t literal;
        start = parseLiteral(literal, binary, start, end);
        packet.value = literal;
    }
    else
    {
        PacketList packets;
        int subPacketListSizeType = binary[start++] - '0';
        switch (subPacketListSizeType)
        {
        case SUB_PACKET_LENGTH:
        {
            int64_t size;
            start = parseInt(size, binary, start, start + SUB_PACKET_LENGTH_SIZE);
            start = parsePacketList(packets, binary, start, start + size);
            break;
        }
        case SUB_PACKET_COUNT:
        {
            int64_t count;
            start = parseInt(count, binary, start, start + SUB_PACKET_COUNT_SIZE);
            start = parsePacketList(packets, count, binary, start, end);
            break;
        }
        default:
            break;
        }
        packet.value = std::move(packets);
    }
        return start;
}

static int64_t sumOfVersionNumbers(Packet const& packet)
{
    int64_t sum = packet.version;
    if (packet.containsPackets())
    {
        PacketList const& packets = std::any_cast<PacketList const&>(packet.value);
        for (auto const & p : packets)
            sum += sumOfVersionNumbers(p);
    }
    return sum;
}

int64_t Packet::evaluate() const
{
    int64_t result = 0;
    switch (type)
    {
    case OPERATOR_SUM:
    {
        PacketList const& packets = std::any_cast<PacketList const&>(value);
        result = 0;
        for (auto const& p : packets)
        {
            result += p.evaluate();
        }
        break;
    }
    case OPERATOR_PRODUCT:
    {
        PacketList const& packets = std::any_cast<PacketList const&>(value);
        result = 1;
        for (auto const& p : packets)
        {
            result *= p.evaluate();
        }
        break;
    }
    case OPERATOR_MIN:
    {
        PacketList const& packets = std::any_cast<PacketList const&>(value);
        result = packets[0].evaluate();
        for (auto p = packets.begin() + 1; p != packets.end(); ++p)
        {
            result = std::min(result, p->evaluate());
        }
        break;
    }
    case OPERATOR_MAX:
    {
        PacketList const& packets = std::any_cast<PacketList const&>(value);
        result = packets[0].evaluate();
        for (auto p = packets.begin() + 1; p != packets.end(); ++p)
        {
            result = std::max(result, p->evaluate());
        }
        break;
    }
    case LITERAL:
    {
       result = std::any_cast<int64_t>(value);
       break;
    }
    case OPERATOR_GREATER:
    {
        PacketList const& packets = std::any_cast<PacketList const&>(value);
        result = packets[0].evaluate() > packets[1].evaluate();
        break;
    }
    case OPERATOR_LESS:
    {
        PacketList const& packets = std::any_cast<PacketList const&>(value);
        result = packets[0].evaluate() < packets[1].evaluate();
        break;
    }
    case OPERATOR_EQUAL:
    {
        PacketList const& packets = std::any_cast<PacketList const&>(value);
        result = packets[0].evaluate() == packets[1].evaluate();
        break;
    }
    default:
        break;
    }
    return result;
}
