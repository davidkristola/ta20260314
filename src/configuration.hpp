#pragma once
/// @file

#include "aircraft_type.hpp"
#include "common_types.hpp"

#include <random>
#include <string>
#include <vector>

namespace ta {

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
    std::vector<ta::AircraftType> aircraft_types = {ALPHA, BRAVO, CHARLIE, DELTA, ECHO};
    AircraftCountType             aircraft_count = 20;
    ChargerCountType              charger_count  = 3;
    HoursType                     run_time       = 3.0;
    SeedType                      seed           = std::random_device()();
    bool                          disable_faults = false;

    void process_command_line_arguments(const std::vector<std::string>& args);
};

} // namespace ta
