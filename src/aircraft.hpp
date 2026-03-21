#pragma once

#include "aircraft_type.hpp"
#include "common_types.hpp"
#include "sim_entity.hpp"

namespace ta {

enum class AircraftState { idle, charging, flying };

class Aircraft : public SimEntity {
    AircraftType      m_type;
    KilowattHoursType m_current_charge;
    AircraftState     m_state               = AircraftState::idle;
    HoursType         m_activity_start_time = 0.0;

public:
    Aircraft() noexcept
        : SimEntity()
        , m_type{}
        , m_current_charge(0.0)
    {}
    Aircraft(AircraftType type_of_craft, SimEntityId id) noexcept;

    // copy constructor
    Aircraft(const Aircraft& other) noexcept;

    // move constructor
    Aircraft(Aircraft&& other) noexcept;

    virtual void process_event(const EventType& e, SharedResources& res) override;

    void fly_for(HoursType time, SharedResources& res) noexcept;
    void charge_for(HoursType time, SharedResources& res) noexcept;

    // Below are methods used internally to process events. They are exposed for unit testing.
    KilowattHoursType current_charge() const noexcept { return m_current_charge; }
    KilowattHoursType depleted_charge() const noexcept { return m_type.m_battery_capacity - m_current_charge; }
    MilesType         flight_distance() const noexcept { return m_current_charge / m_type.m_energy_used_at_cruise; }
    HoursType         flight_time() const noexcept { return (flight_distance() / m_type.m_cruise_speed); }
    HoursType         charge_time() const noexcept { return (depleted_charge() / m_type.charge_rate()); }
    AircraftState     state() const noexcept { return m_state; }
};

} // namespace ta
