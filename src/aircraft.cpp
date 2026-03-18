#include "aircraft.hpp"

namespace ta {

Aircraft::Aircraft(AircraftType type_of_craft, AircraftId id) noexcept
    : m_type(type_of_craft), m_id(id), m_current_charge(type_of_craft.m_battery_capacity) {}

HoursType Aircraft::charge_time() const noexcept {
    const auto charge_remaining = (m_type.m_battery_capacity - m_current_charge);
    return (charge_remaining / m_type.charge_rate());
}

void Aircraft::fly_for(HoursType time) noexcept {
    m_current_time += time;
    const MilesType distance = (time * m_type.m_cruise_speed);
    m_current_charge -= (distance / m_type.m_energy_used_at_cruise);
    if (m_current_charge < 0.0) {
        m_current_charge = 0.0;
    }
}

void Aircraft::charge_for(HoursType time) noexcept {
    m_current_time += time;
    const auto charge_amount = (time * m_type.charge_rate());
    m_current_charge += charge_amount;
    if (m_current_charge > m_type.m_battery_capacity) {
        m_current_charge = m_type.m_battery_capacity;
    }
}

void Aircraft::delay_for(HoursType time) noexcept { m_current_time += time; }

} // namespace ta
