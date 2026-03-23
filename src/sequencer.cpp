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

Sequencer::Sequencer(Configuration configuration) noexcept
    : m_aircraft_types(configuration.aircraft_types)
    , m_vertiport{VERTIPORT_ID, configuration.charger_count}
{
    m_shared.m_fault_model.seed(configuration.seed);
    m_shared.m_vertiport_id = VERTIPORT_ID;
    m_shared.m_queue.push(EventType{configuration.run_time, Cause::end_of_simulation});
    m_shared.disable_faults = configuration.disable_faults;

    for (SimEntityId id = 1U; id <= configuration.aircraft_count; ++id) {
        Aircraft aircraft{pick_type(id), id};
        m_aircraft.emplace(id, aircraft);
        EventType takeoff_event{m_current_time, Cause::take_off, id};
        m_shared.m_queue.push(takeoff_event);
    }

    for (const auto& t : m_aircraft_types) {
        m_shared.m_statistics.add(Statistics{t});
    }

    for (auto [k, v] : m_aircraft) {
        const auto t = v.get_type();
        auto&      s = m_shared.m_statistics.get(t.m_name);
        s.record_vehicle(k);
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

    case Cause::experience_fault: {
        target->process_event(event, m_shared);
    } break;

    default:
        break;
    }
    if (not(m_done or m_shared.m_queue.empty())) {
        m_current_time = m_shared.m_queue.top_time();
    }
}

const AircraftType& Sequencer::pick_type(SimEntityId id) noexcept
{
    if ((id <= m_aircraft_types.size()) or (m_shared.disable_faults)) {
        return m_aircraft_types[(id - 1) % m_aircraft_types.size()];
    }
    return m_aircraft_types[m_shared.m_fault_model.random_index(m_aircraft_types.size() - 1)];
}

} // namespace ta
