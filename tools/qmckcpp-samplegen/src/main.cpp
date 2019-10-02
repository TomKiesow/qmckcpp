#include <iostream>
#include <random>
#include <bitset>
#include <fstream>
#include <cstdint>

void print_usage();

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        print_usage();
        return -1;
    }

    std::uint32_t inputs{(std::uint32_t) std::stoul(argv[1])};
    std::uint32_t outputs{(std::uint32_t) std::stoul(argv[2])};
    std::ofstream file{argv[3]};

    file << "inputs = " << inputs << '\n';
    file << "outputs = " << outputs << '\n';

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 1); // define the range

    std::uint32_t counter = 0;
    std::uint32_t limit = 1u << inputs;
    while (counter++ < limit)
    {
        for (std::uint32_t i = 0; i < inputs; ++i)
        {
            file << ((counter >> (inputs - i - 1)) & 1u);
        }
        file << '|';
        for (std::uint32_t i = 0; i < outputs; ++i)
        {
            file << distr(eng);
        }
        file << ";\n";
    }
}

void print_usage()
{
    std::cout << "usage: qmckcpp-smaplegen <input count> <output count> <filename>\n";
}
