/// @file

#include "simulation.hpp"

#include <iostream>

namespace ta {

void Simulation::setup_and_run(const std::vector<std::string>& args)
{
    m_configuration.process_command_line_arguments(args);
    Sequencer sequencer{m_configuration};
    std::cout << "\nrunning with seed " << m_configuration.seed << "\n";
    while (not sequencer.done()) {
        sequencer.step();
    }
    std::cout << "\n";
    report(sequencer);
}

void Simulation::report(Sequencer& simulation)
{
    std::cout << "\n******** Statistics ********\n";
    for (const auto& vehicle_type : m_configuration.aircraft_types) {
        show_statistics_for(vehicle_type.m_name, simulation.statistics(vehicle_type.m_name));
    }
}
void Simulation::show_statistics_for(const std::string_view& vehicle_type_name, const Statistics& statistics)
{
    std::cout << "Statistics for " << vehicle_type_name << "\n";
    std::cout << "    average flight time per flight          : " << statistics.average_hours_per_flight() << "\n";
    std::cout << "    average distance traveled per flight    : " << statistics.average_miles_per_flight() << "\n";
    std::cout << "    average time charging per charge session: " << statistics.average_hours_per_charge() << "\n";
    std::cout << "    total number of faults                  : " << statistics.total_faults() << "\n";
    std::cout << "    total number of passenger miles         : " << statistics.total_passenger_miles() << "\n";
    std::cout << "    total vehicles in sim                   : " << statistics.total_vehicles() << "\n";
}
} // namespace ta
