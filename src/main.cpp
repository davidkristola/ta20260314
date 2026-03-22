

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

unsigned long seed = 0U;

void process_command_line_arguments(const std::vector<std::string>& args)
{
    for (size_t i = 1U; i < args.size(); ++i) {
        if (args[i] == "--seed" && i + 1 < args.size()) {
            std::stringstream seed_buffer(args[++i]);
            if (seed_buffer >> seed) {
                std::cout << " (Seed=" << seed << ")";
            } else {
                std::cout << " (Invalid Seed='" << args[i] << "')";
            }
        } else {
            std::cout << " " << args[i];
        }
    }
}

constexpr ta::AircraftType ALPHA = {.m_name                       = "Alpha",
                                    .m_cruise_speed               = 120.0,
                                    .m_battery_capacity           = 320.0,
                                    .m_time_to_charge             = 0.6,
                                    .m_energy_used_at_cruise      = 1.6,
                                    .m_passenger_count            = 4,
                                    .m_fault_probability_per_hour = 0.25};

constexpr ta::AircraftType BRAVO = {.m_name                       = "Bravo",
                                    .m_cruise_speed               = 100.0,
                                    .m_battery_capacity           = 100.0,
                                    .m_time_to_charge             = 0.2,
                                    .m_energy_used_at_cruise      = 1.5,
                                    .m_passenger_count            = 5,
                                    .m_fault_probability_per_hour = 0.10};

constexpr ta::AircraftType CHARLIE = {.m_name                       = "Charlie",
                                      .m_cruise_speed               = 160.0,
                                      .m_battery_capacity           = 220.0,
                                      .m_time_to_charge             = 0.8,
                                      .m_energy_used_at_cruise      = 2.2,
                                      .m_passenger_count            = 3,
                                      .m_fault_probability_per_hour = 0.05};

constexpr ta::AircraftType DELTA = {.m_name                       = "Delta",
                                    .m_cruise_speed               = 90.0,
                                    .m_battery_capacity           = 120.0,
                                    .m_time_to_charge             = 0.62,
                                    .m_energy_used_at_cruise      = 0.8,
                                    .m_passenger_count            = 2,
                                    .m_fault_probability_per_hour = 0.22};

constexpr ta::AircraftType ECHO = {.m_name                       = "Echo",
                                   .m_cruise_speed               = 30.0,
                                   .m_battery_capacity           = 150.0,
                                   .m_time_to_charge             = 0.3,
                                   .m_energy_used_at_cruise      = 5.8,
                                   .m_passenger_count            = 2,
                                   .m_fault_probability_per_hour = 0.61};

struct Configuration {
    std::vector<ta::AircraftType> aircraft_types{ALPHA, BRAVO, CHARLIE, DELTA, ECHO};
    ta::AircraftCountType         ac       = 20;
    ta::ChargerCountType          cc       = 3;
    ta::HoursType                 run_time = 3.0;
};

void show_statistics_for(const ta::AircraftType& vehicle_type, ta::Sequencer& simulation)
{
    const ta::Statistics s{simulation.statistics(vehicle_type.m_name)};
    std::cout << "Statistics for " << vehicle_type.m_name << "\n";
    std::cout << "    average flight time per flight          : " << s.average_hours_per_flight() << "\n";
    std::cout << "    average distance traveled per flight    : " << s.average_miles_per_flight() << "\n";
    std::cout << "    average time charging per charge session: " << s.average_hours_per_charge() << "\n";
    std::cout << "    total number of faults                  : " << s.total_faults() << "\n";
    std::cout << "    total number of passenger miles         : " << s.total_passenger_miles() << "\n";
}

void show_all_statistics(const Configuration& c, ta::Sequencer& simulation)
{
    std::cout << "\n******** Statistics ********\n";
    for (const auto& vehicle_type : c.aircraft_types) {
        show_statistics_for(vehicle_type, simulation);
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    process_command_line_arguments(args);
    std::cout << "\nrunning:";
    Configuration c;
    ta::Sequencer simulation{c.aircraft_types, c.ac, c.cc, c.run_time};
    while (not simulation.done()) {
        simulation.step();
    }
    std::cout << "\n";
    show_all_statistics(c, simulation);
    return 0;
}
