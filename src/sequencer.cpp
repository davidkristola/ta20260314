#include "sequencer.hpp"

namespace {
constexpr ta::SimEntityId VERTIPORT_ID = 9999U;
}

namespace ta {
Sequencer::Sequencer(std::vector<AircraftType> aircraft_types, AircraftCountType ac, ChargerCountType cc,
                     HoursType run_time) noexcept
    : m_aircraft_types(aircraft_types)
    , m_vertiport{VERTIPORT_ID, cc}
{
    m_shared.m_vertiport_id = VERTIPORT_ID;
    m_shared.m_queue.push(EventType{run_time, Cause::end_of_simulation});

    for (SimEntityId id = 1U; id <= ac; ++id) {
        Aircraft aircraft{pick_type(id), id};
        m_aircraft.emplace(id, aircraft);
        EventType takeoff_event{m_current_time, Cause::take_off, id};
        m_shared.m_queue.push(takeoff_event);
    }

    for (const auto& t : m_aircraft_types) {
        m_shared.m_statistics.add(Statistics{t});
    }
}

Statistics Sequencer::statistics(std::string_view type_name) { return m_shared.m_statistics.get(type_name); }

void Sequencer::step() noexcept
{
    if (m_shared.m_queue.empty()) {
        m_done = true;
        return;
    }
    if (m_done) {
        return;
    }
    const auto event     = m_shared.m_queue.pop();
    const auto target_id = event.target();
    SimEntity* target    = nullptr;
    if (target_id == m_shared.m_vertiport_id) {
        target = &m_vertiport;
    } else {
        target = &m_aircraft[target_id];
    }
    switch (event.cause()) {
    case Cause::take_off: {
        target->process_event(event, m_shared);
    } break;

    case Cause::land: {
        target->process_event(event, m_shared);
    } break;

    case Cause::start_charging: {
        target->process_event(event, m_shared);
    } break;

    case Cause::complete_charging: {
        target->process_event(event, m_shared);
    } break;

    case Cause::end_of_simulation: {
        m_done = true;
    } break;

    default:
        // TODO(djk): add/handle fault
        break;
    }
    if (not(m_done or m_shared.m_queue.empty())) {
        m_current_time = m_shared.m_queue.top_time();
    }
}

const AircraftType& Sequencer::pick_type(SimEntityId id) const noexcept
{
    // TODO(djk): This is one of two places where random numbers need to be used
    return m_aircraft_types[(id - 1) % m_aircraft_types.size()];
}

} // namespace ta
