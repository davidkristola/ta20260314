#pragma once

#include "aircraft_type.hpp"


namespace ta {

class Aircraft {
    AircraftType m_type;
    AircraftId m_id;
    KilowattHoursType m_current_charge;
    HoursType m_current_time = 0.0;

public:
    Aircraft(AircraftType type_of_craft, AircraftId id) noexcept;
    KilowattHoursType current_charge() const noexcept { return m_current_charge; }
    MilesType flight_distance() const noexcept { return m_current_charge / m_type.m_energy_used_at_cruise; }
    HoursType flight_time() const noexcept { return (flight_distance() / m_type.m_cruise_speed); }
    HoursType charge_time() const noexcept;
    HoursType current_time() const noexcept { return m_current_time; }
    void fly_for(HoursType time) noexcept;
    void charge_for(HoursType time) noexcept;
    void delay_for(HoursType time) noexcept;
};

} // namespace ta
