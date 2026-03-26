/// @file

#include "configuration.hpp"

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace ta {

bool match_with_payload(std::string what, size_t i, const std::vector<std::string>& args)
{
    return (args[i] == what && i + 1 < args.size());
}

template <typename T> T extract_payload(std::string arg, T or_return_this)
{
    std::stringstream seed_buffer(arg);
    T                 temp = 0;
    if (seed_buffer >> temp) {
        return temp;
    }
    return or_return_this;
}

void show_help()
{
    std::cout << "command line options:\n";
    std::cout << " --help    print this info.\n";
    std::cout << " --seed #  set the sim run seed to # (unsigned int in decimal).\n";
    std::cout << " --rt #    set the sim run time to # (double hours).\n";
    std::cout << " --ac #    set the sim aircraft count to # (unsigned int in decimal).\n";
    std::cout << " --cc #    set the sim charger count to # (unsigned int in decimal).\n";
    std::cout << " --df      disable faults.\n";
}

void Configuration::process_command_line_arguments(const std::vector<std::string>& args)
{
    for (size_t i = 1U; i < args.size(); ++i) {
        if (match_with_payload("--seed", i, args)) {
            seed = extract_payload<unsigned int>(args[++i], seed);
        } else if (match_with_payload("--rt", i, args)) {
            run_time = extract_payload<HoursType>(args[++i], run_time);
        } else if (match_with_payload("--cc", i, args)) {
            charger_count = extract_payload<ChargerCountType>(args[++i], charger_count);
        } else if (match_with_payload("--ac", i, args)) {
            aircraft_count = extract_payload<AircraftCountType>(args[++i], aircraft_count);
        } else if (args[i] == "--df") {
            disable_faults = true;
        } else if (args[i] == "--help") {
            show_help();
        }
    }
}

} // namespace ta
