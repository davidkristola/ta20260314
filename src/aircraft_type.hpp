#pragma once

#include "common_types.hpp"

#include <string_view>

namespace ta {

// TODO(djk): make this an immutable class
struct AircraftType {
    std::string_view m_name                             = "";
    MilesPerHourType m_cruise_speed                     = 0.0;
    KilowattHoursType m_battery_capacity                = 0.0;
    HoursType m_time_to_charge                          = 0.0;
    KilowattHoursPerMileType m_energy_used_at_cruise    = 0.0;
    PassengerCountType m_passenger_count                = 0U;
    ProbabilityPerHourType m_fault_probability_per_hour = 0.0;
    constexpr KilowattHoursPerHourType charge_rate() const noexcept { return (m_battery_capacity / m_time_to_charge); }
};

} // namespace ta
