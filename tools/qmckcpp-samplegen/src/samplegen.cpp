#include <iostream>
#include <random>
#include <bitset>
#include <fstream>
#include <cstdint>

#include <string>

void print_usage();

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        print_usage();
        return -1;
    }

    const uint32_t inputs = std::stoul(argv[1]);
    const uint32_t outputs = std::stoul(argv[2]);
    std::ofstream file{argv[3]};

    file << "inputs = " << inputs << '\n';
    file << "outputs = " << outputs << '\n';

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_int_distribution<uint32_t> distrib{0u, 1u};

    uint32_t limit = 1u << inputs;
    for (uint32_t c = 1u; c <= limit; ++c)
    {
        for (uint32_t j = 0u; j < inputs; ++j)
        {
            file << ((c >> (inputs - j - 1u)) & 1u);
        }
        file << '|';
        for (uint32_t j = 0u; j < outputs; ++j)
        {
            file << distrib(engine);
        }
        file << ";\n";
    }
}

void print_usage()
{
    std::cout << "usage: qmckcpp-smaplegen <input count> <output count> <filename>\n";
}
