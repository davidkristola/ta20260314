#pragma once

#include "aircraft_type.hpp"


namespace ta {

class Aircraft {
    AircraftType m_type;
    AircraftId m_id;
    KilowattsPerHourType m_current_charge;

public:
    Aircraft(AircraftType type_of_craft, AircraftId id) noexcept;
    KilowattsPerHourType current_charge() const noexcept { return m_current_charge; }
};

} // namespace ta
