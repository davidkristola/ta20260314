#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <sstream>

// https://extapps.ksc.nasa.gov/Reliability/Documents/170505_Risk_Failure_Probability_and_Failure_Rate.pdf

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    std::cout << "running:";
    for (size_t i = 1U; i < args.size(); ++i) {
        if (args[i] == "--seed" && i + 1 < args.size()) {
            std::stringstream seed_buffer(args[++i]);
            int seed = 0;
            if (seed_buffer >> seed) {
                std::cout << " (Seed=" << seed << ")";
            } else {
                std::cout << " (Invalid Seed='" << args[i] << "')";
            }
        }
        else {
            std::cout << " " << args[i];
        }
    }
    std::cout << "\n";
    return 0;
}
