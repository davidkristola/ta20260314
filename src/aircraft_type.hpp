#pragma once

#include "common_types.hpp"

#include <string_view>

namespace ta {

// TODO(djk): make this an innutable class
struct AircraftType {
    std::string_view m_name                             = "";
    MilesPerHourType m_cruise_speed                     = 0.0;
    KilowattsPerHourType m_battery_capacity             = 0.0;
    HoursType m_time_to_charge                          = 0.0;
    KilowattsPerHourPerMileType m_energy_used_at_cruise = 0.0;
    PassengerCountType m_passenger_count                = 0U;
    ProbabilityType m_fault_probability_per_hour        = 0.0;
};

} // namespace ta
