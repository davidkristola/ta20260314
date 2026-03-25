/// @file
#include "aircraft.hpp"
#include "event_queue.hpp"
#include "event_type.hpp"
#include "shared_resources.hpp"

namespace ta {

Aircraft::Aircraft(AircraftType type_of_craft, SimEntityId id) noexcept
    : SimEntity(id)
    , m_type(type_of_craft)
    , m_current_charge(type_of_craft.m_battery_capacity)
{}

Aircraft::Aircraft(const Aircraft& other) noexcept
    : SimEntity(other)
    , m_type(other.m_type)
    , m_current_charge(other.m_current_charge)
    , m_state(other.m_state)
{}

Aircraft::Aircraft(Aircraft&& other) noexcept
    : SimEntity(std::move(other))
    , m_type(other.m_type)
    , m_current_charge(other.m_current_charge)
    , m_state(other.m_state)
{}

void Aircraft::fly_for(HoursType time, Statistics& statistics) noexcept
{
    const MilesType distance = (time * m_type.m_cruise_speed);
    m_current_charge -= (distance * m_type.m_energy_used_at_cruise);
    if (m_current_charge < 0.0) {
        m_current_charge = 0.0;
    }
    statistics.record_flight(time, distance, m_type.m_passenger_count);
}

void Aircraft::charge_for(HoursType time, Statistics& statistics) noexcept
{
    const auto charge_amount = (time * m_type.charge_rate());
    m_current_charge += charge_amount;
    if (m_current_charge > m_type.m_battery_capacity) {
        m_current_charge = m_type.m_battery_capacity;
    }
    statistics.record_charging_session(time);
}

void Aircraft::process_event(const EventType& event, SharedResources& resources)
{
    switch (event.cause()) {
    case Cause::take_off: {
        process_take_off(event, resources);
    } break;

    case Cause::land: {
        process_landing(event, resources);
    } break;

    case Cause::start_charging: {
        process_charging(event, resources);
    } break;

    case Cause::complete_charging: {
        process_charging_complete(event, resources);
    } break;

    case Cause::experience_fault: {
        process_fault(resources.aircraft_statistics(m_type.m_name));
    } break;

    default:
        break;
    }
}

void Aircraft::process_fault(Statistics& statistics)
{
    // TODO: Should this record flight time, miles, and passenger miles? Assume not until resolved.
    m_state = AircraftState::faulted;
    statistics.record_fault();
}

void Aircraft::process_charging_complete(const EventType& event, SharedResources& resources)
{
    m_state = AircraftState::idle;
    charge_for(event.time() - m_activity_start_time, resources.aircraft_statistics(m_type.m_name));
    m_activity_start_time = event.time();
    // Now automatically fly again (this could be managed externally by some sort of dispatcher or at least
    // configurable).
    resources.push({event.time(), Cause::take_off, id(), resources.vertiport_id()});
}

void Aircraft::process_charging(const EventType& event, SharedResources& resources)
{
    m_state = AircraftState::charging;
    resources.push({event.time() + charge_time(), Cause::complete_charging, id()});
    m_activity_start_time = event.time();
}

void Aircraft::process_landing(const EventType& event, SharedResources& resources)
{
    m_state = AircraftState::idle;
    fly_for(event.time() - m_activity_start_time, resources.aircraft_statistics(m_type.m_name));
    m_activity_start_time = event.time();
    resources.push({event.time(), Cause::land, resources.vertiport_id(), id()});
}

void Aircraft::process_take_off(const EventType& event, SharedResources& resources)
{
    m_state               = AircraftState::flying;
    const auto fault_time = next_fault_time(resources);
    if (resources.faults_enabled() and (fault_time < flight_time())) {
        // post a fault
        resources.push({event.time() + fault_time, Cause::experience_fault, id()});
    } else {
        // post the landing
        resources.push({event.time() + flight_time(), Cause::land, id(), event.secondary_object()});
    }
    m_activity_start_time = event.time();
}

HoursType Aircraft::next_fault_time(SharedResources& resources)
{
    return resources.m_fault_model.time_to_next_fault(m_type.m_fault_probability_per_hour);
}

} // namespace ta
