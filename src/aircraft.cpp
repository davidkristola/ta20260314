#include "aircraft.hpp"

namespace ta {

Aircraft::Aircraft(AircraftType type_of_craft, AircraftId id) noexcept
    : m_type(type_of_craft), m_id(id), m_current_charge(type_of_craft.m_battery_capacity) {}

} // namespace ta
