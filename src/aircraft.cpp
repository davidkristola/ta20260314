#include "aircraft.hpp"

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
    , m_current_time(other.m_current_time)
    , m_state(other.m_state)
{}

Aircraft::Aircraft(Aircraft&& other) noexcept
    : SimEntity(std::move(other))
    , m_type(other.m_type)
    , m_current_charge(other.m_current_charge)
    , m_current_time(other.m_current_time)
    , m_state(other.m_state)
{}

HoursType Aircraft::charge_time() const noexcept
{
    const auto charge_remaining = (m_type.m_battery_capacity - m_current_charge);
    return (charge_remaining / m_type.charge_rate());
}

void Aircraft::fly_for(HoursType time, SharedResources& res) noexcept
{
    m_current_time += time;
    const MilesType distance = (time * m_type.m_cruise_speed);
    m_current_charge -= (distance / m_type.m_energy_used_at_cruise);
    if (m_current_charge < 0.0) {
        m_current_charge = 0.0;
    }
    auto& s = res.m_statistics.get(m_type.m_name);
    s.record_flight(time, distance, m_type.m_passenger_count);
}

void Aircraft::charge_for(HoursType time, SharedResources& res) noexcept
{
    m_current_time += time;
    const auto charge_amount = (time * m_type.charge_rate());
    m_current_charge += charge_amount;
    if (m_current_charge > m_type.m_battery_capacity) {
        m_current_charge = m_type.m_battery_capacity;
    }
    auto& s = res.m_statistics.get(m_type.m_name);
    s.record_charging_session(time);
}

void Aircraft::delay_for(HoursType time) noexcept { m_current_time += time; }

void Aircraft::process_event(const EventType& e, SharedResources& res)
{
    switch (e.cause()) {
    case Cause::take_off: {
        m_state = AircraftState::flying;
        // TODO(djk): inject faults
        EventType land_event{e.time() + flight_time(), Cause::land, id()};
        res.m_queue.push(land_event);
        m_activity_start_time = e.time();
    } break;
    case Cause::land: {
        m_state = AircraftState::idle;
        fly_for(e.time() - m_activity_start_time, res);
        m_activity_start_time = e.time();
    } break;
    case Cause::start_charging: {
        m_state = AircraftState::charging;
        EventType event{e.time() + charge_time(), Cause::complete_charging, id()};
        res.m_queue.push(event);
        m_activity_start_time = e.time();
    } break;
    case Cause::complete_charging: {
        m_state = AircraftState::idle;
        charge_for(e.time() - m_activity_start_time, res);
        m_activity_start_time = e.time();
    } break;

    default:
        break;
    }
}

} // namespace ta
