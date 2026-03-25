#pragma once
/// @file

#include "aircraft_type.hpp"
#include "common_types.hpp"
#include "sim_entity.hpp"
#include "statistics.hpp"

namespace ta {

enum class AircraftState { idle, charging, flying, faulted };

/// @brief A representation of an eVTOL aircraft simulation object.
/// @details This is a simulation entity with a unique ID and a type of aircraft.
/// @startuml{aircraft_state_machine.png} "State Machine"
/// [*] --> idle
/// idle --> flying
/// idle --> charging
/// flying --> idle
/// flying --> faulted
/// charging --> idle
/// faulted --> [*]
/// @enduml
//
class Aircraft : public SimEntity {
    AircraftType      m_type;
    KilowattHoursType m_current_charge;
    AircraftState     m_state               = AircraftState::idle;
    HoursType         m_activity_start_time = 0.0;

public:
    /// Default constructor.
    Aircraft() noexcept
        : SimEntity()
        , m_type{}
        , m_current_charge(0.0)
    {}

    /// Primary constructor.
    Aircraft(AircraftType type_of_craft, SimEntityId id) noexcept;

    /// Copy constructor.
    Aircraft(const Aircraft& other) noexcept;

    /// Move constructor.
    Aircraft(Aircraft&& other) noexcept;

    /// @brief Process this aircraft'statistics event.
    /// @param event The event to process.
    /// @param resources Shared resources needed to process some events.
    //
    virtual void process_event(const EventType& event, SharedResources& resources) override;

    /// This method updates aircraft resources and records activity.
    //
    void fly_for(HoursType time, Statistics& statistics) noexcept;

    /// This method updates aircraft resources and records activity.
    //
    void charge_for(HoursType time, Statistics& statistics) noexcept;

    ///@{
    /// Internal @c const method to facilitate event processing. Exposed for unit testing.
    const AircraftType& get_type() const noexcept { return m_type; }

    KilowattHoursType current_charge() const noexcept { return m_current_charge; }
    KilowattHoursType depleted_charge() const noexcept { return m_type.m_battery_capacity - m_current_charge; }
    MilesType         flight_distance() const noexcept { return m_current_charge / m_type.m_energy_used_at_cruise; }
    HoursType         flight_time() const noexcept { return (flight_distance() / m_type.m_cruise_speed); }
    HoursType         charge_time() const noexcept { return (depleted_charge() / m_type.charge_rate()); }
    AircraftState     state() const noexcept { return m_state; }
    ///@}

private:
    /// @brief Handle the @c Cause::experience_fault event.
    /// @startuml
    /// ?-> aircraft : process_event(Cause::experience_fault)
    /// activate aircraft
    /// aircraft -> aircraft : process_fault()
    /// note right
    ///  Transition to faulted state
    /// end note
    /// deactivate aircraft
    /// hide footbox
    /// @enduml
    /// @param statistics Statistics object for tracking this type of aircraft's statistics operating statistics.
    //
    void process_fault(Statistics& statistics);

    /// @brief Handle the @c Cause::complete_charging event.
    /// @startuml
    /// ?-> aircraft : process_event(Cause::complete_charging)
    /// activate aircraft
    /// aircraft -> aircraft : process_charging_complete()
    /// activate aircraft
    /// aircraft -> event_queue : push(Cause::process_charging_complete)
    /// note right
    ///  Send event to the vertiport
    /// end note
    /// deactivate aircraft
    /// deactivate aircraft
    /// hide footbox
    /// @enduml
    /// @param event The event to process.
    /// @param resources Shared resources.
    //
    void process_charging_complete(const EventType& event, SharedResources& resources);

    /// @brief Handle the @c Cause::start_charging event.
    /// @startuml
    /// ?-> aircraft : process_event(Cause::start_charging)
    /// activate aircraft
    /// aircraft -> aircraft : process_charging()
    /// activate aircraft
    /// aircraft -> event_queue : push(Cause::complete_charging)
    /// note right
    ///  Send event to future self
    /// end note
    /// deactivate aircraft
    /// deactivate aircraft
    /// hide footbox
    /// @enduml
    /// @param event The event to process.
    /// @param resources Shared resources.
    //
    void process_charging(const EventType& event, SharedResources& resources);

    /// @brief Handle the @c Cause::land event.
    /// @startuml
    /// ?-> aircraft : process_event(Cause::land)
    /// activate aircraft
    /// aircraft -> aircraft : process_landing()
    /// activate aircraft
    /// aircraft -> event_queue : push(Cause::land)
    /// note right
    ///  Send event to the vertiport
    /// end note
    /// deactivate aircraft
    /// deactivate aircraft
    /// hide footbox
    /// @enduml
    /// @param event The event to process.
    /// @param resources Shared resources.
    //
    void process_landing(const EventType& event, SharedResources& resources);

    /// Handle the @c Cause::take_off event.
    /// Nominally, this method will post a @c land event at the appropriate time.
    /// If fault processing is enabled and a fault would occur during the flight,
    /// an @c Cause::experience_fault event is posted instead of a @c Cause::land event.
    /// @startuml
    /// ?-> aircraft : process_event(Cause::take_off)
    /// activate aircraft
    /// aircraft -> aircraft : process_take_off()
    /// activate aircraft
    /// alt no fault
    /// aircraft -> event_queue : push(Cause::land)
    /// note right
    ///  Send event to future self
    /// end note
    /// else fault
    /// aircraft -> event_queue : push(Cause::experience_fault)
    /// note right
    ///  Send event to future self
    /// end note
    /// end
    /// deactivate aircraft
    /// deactivate aircraft
    /// hide footbox
    /// @enduml
    /// @param event The event to process.
    /// @param resources Shared resources.
    //
    void process_take_off(const EventType& event, SharedResources& resources);

    /// Compute the flight time of the next fault based on the aircraft type fault probability.
    //
    HoursType next_fault_time(SharedResources& resources);
};

} // namespace ta
