#pragma once

#include "aircraft_type.hpp"
#include "event_queue.hpp"
#include "event_type.hpp"
#include "shared_resources.hpp"

namespace ta {

class SimEntity {
protected:
    SimEntityId m_id = InvalidSimEntityId;

public:
    SimEntity() noexcept {}
    explicit SimEntity(SimEntityId id) noexcept
        : m_id(id)
    {}
    virtual ~SimEntity() = default;

    // copy constructor
    SimEntity(const SimEntity& other) noexcept = default;

    // move constructor
    SimEntity(SimEntity&& other) noexcept = default;

    SimEntityId id() const noexcept { return m_id; }

    virtual void process_event(const EventType& e, SharedResources& res) = 0;
};

enum class AircraftState { idle, charging, flying };

class Aircraft : public SimEntity {
    AircraftType      m_type;
    KilowattHoursType m_current_charge;
    HoursType         m_current_time        = 0.0;
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

    KilowattHoursType current_charge() const noexcept { return m_current_charge; }
    MilesType         flight_distance() const noexcept { return m_current_charge / m_type.m_energy_used_at_cruise; }
    HoursType         flight_time() const noexcept { return (flight_distance() / m_type.m_cruise_speed); }
    HoursType         charge_time() const noexcept;
    HoursType         current_time() const noexcept { return m_current_time; }
    AircraftState     state() const noexcept { return m_state; }

    void fly_for(HoursType time, SharedResources& res) noexcept;
    void charge_for(HoursType time, SharedResources& res) noexcept;
    void delay_for(HoursType time) noexcept;

    virtual void process_event(const EventType& e, SharedResources& res) override;
};

} // namespace ta
