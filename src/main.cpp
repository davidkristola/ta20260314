/// @file

#include "aircraft.hpp"
#include "common_types.hpp"
#include "event_queue.hpp"
#include "sequencer.hpp"
#include "shared_resources.hpp"
#include "statistics.hpp"
#include "vertiport.hpp"



#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// https://extapps.ksc.nasa.gov/Reliability/Documents/170505_Risk_Failure_Probability_and_Failure_Rate.pdf

namespace ta {

void show_statistics_for(const std::string_view& vehicle_type_name, const Statistics& statistics)
{
    std::cout << "Statistics for " << vehicle_type_name << "\n";
    std::cout << "    average flight time per flight          : " << statistics.average_hours_per_flight() << "\n";
    std::cout << "    average distance traveled per flight    : " << statistics.average_miles_per_flight() << "\n";
    std::cout << "    average time charging per charge session: " << statistics.average_hours_per_charge() << "\n";
    std::cout << "    total number of faults                  : " << statistics.total_faults() << "\n";
    std::cout << "    total number of passenger miles         : " << statistics.total_passenger_miles() << "\n";
    std::cout << "    total vehicles in sim                   : " << statistics.total_vehicles() << "\n";
}

void show_all_statistics(const Configuration& configuration, Sequencer& simulation)
{
    std::cout << "\n******** Statistics ********\n";
    for (const auto& vehicle_type : configuration.aircraft_types) {
        show_statistics_for(vehicle_type.m_name, simulation.statistics(vehicle_type.m_name));
    }
}

} // namespace ta

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    ta::Configuration        configuration;
    configuration.process_command_line_arguments(args);
    std::cout << "\nrunning with seed " << configuration.seed << "\n";
    ta::Sequencer simulation{configuration};
    simulation.shared_resources().m_fault_model.seed(configuration.seed);
    while (not simulation.done()) {
        simulation.step();
    }
    std::cout << "\n";
    show_all_statistics(configuration, simulation);
    return 0;
}
