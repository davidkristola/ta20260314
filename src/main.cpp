/// @file

#include "simulation.hpp"

#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    ta::Simulation simulation{};
    simulation.setup_and_run({argv, argv + argc});
    return 0;
}
