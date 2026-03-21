#include "sequencer.hpp"

namespace ta {
Sequencer::Sequencer(std::vector<AircraftType> aircraft_types, AircraftCountType ac, ChargerCountType cc,
                     HoursType run_time) noexcept
    : m_aircraft_types(aircraft_types)
{
    m_shared.m_queue.push(EventType{run_time, Cause::end_of_simulation});

    for (SimEntityId id = 1U; id <= ac; ++id) {
        Aircraft temp_ac{pick_type(id), id};
        m_aircraft.emplace(id, temp_ac);
        EventType takeoff_event{m_current_time, Cause::take_off, id};
        m_shared.m_queue.push(takeoff_event);
    }
    for (SimEntityId id = 1U; id <= cc; ++id) {
        m_chargers.push_back({id});
    }
    for (const auto& t : m_aircraft_types) {
        m_shared.m_statistics.add(Statistics{t});
    }
}

bool      Sequencer::done() const noexcept { return false; }
HoursType Sequencer::simulation_time() const noexcept { return m_current_time; }

Statistics Sequencer::statistics(std::string_view type_name) { return m_shared.m_statistics.get(type_name); }
void       Sequencer::step() noexcept
{
    const auto event = m_shared.m_queue.pop();
    const auto ac_id = event.aircraft();
    auto&      ac    = m_aircraft[ac_id];
    switch (event.cause()) {
    case Cause::take_off: {
        ac.process_event(event, m_shared);
    } break;
    case Cause::land: {
        ac.process_event(event, m_shared);
        // TODO(djk): start charging (introduce a vertiport simulation entity and dispatch event there)
        EventType start_charging_event{event.time(), Cause::start_charging, event.aircraft()};
        m_shared.m_queue.push(start_charging_event);
    } break;
    case Cause::start_charging: {
        ac.process_event(event, m_shared);
    } break;
    case Cause::complete_charging: {
        ac.process_event(event, m_shared);
    } break;
    case Cause::end_of_simulation: {

    } break;

    default:
        // TODO(djk): add/handle fault
        break;
    }
    m_current_time = m_shared.m_queue.top_time();
}

const AircraftType& Sequencer::pick_type(SimEntityId id) const noexcept
{
    // TODO(djk): This is one of two places where random numbers need to be used
    return m_aircraft_types[0];
}

} // namespace ta
