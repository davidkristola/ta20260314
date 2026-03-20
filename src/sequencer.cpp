#include "sequencer.hpp"

namespace ta {
Sequencer::Sequencer(std::vector<AircraftType> aircraft_types, AircraftCountType ac, ChargerCountType cc,
                     HoursType run_time) noexcept
    : m_aircraft_types(aircraft_types) //
{
    m_queue.push(EventType{m_next_event_id++, Cause::end_of_simulation, run_time});
    for (AircraftId id = 1U; id <= ac; ++id) {
        m_aircraft.push_back({m_aircraft_types[0], id});
        EventType takeoff_event{m_next_event_id++, Cause::take_off, m_current_time};
        takeoff_event.set_aircraft(id);
        m_queue.push(takeoff_event);
    }
    for (ChargerId id = 1U; id <= cc; ++id) {
        m_chargers.push_back({id});
    }
    for (const auto& t : m_aircraft_types) {
        m_statistics.push_back(Statistics{t});
    }
}

bool Sequencer::done() const noexcept { return false; }
HoursType Sequencer::simulation_time() const noexcept { return m_current_time; }

std::optional<const Statistics*> Sequencer::statistics(std::string_view type_name) const {
    for (const auto& s : m_statistics) {
        if (s.name() == type_name) {
            return &s;
        }
    }
    return std::optional<const Statistics*>{};
}
void Sequencer::step() noexcept {
    const auto event = m_queue.pop();
    switch (event.cause()) {
    case Cause::take_off: {
        const auto ac_id       = event.aircraft();
        const auto& ac         = m_aircraft[ac_id.value() - 1];
        const auto flight_time = ac.flight_time();
        // TODO(djk): inject faults
        EventType land_event{m_next_event_id++, Cause::land, m_current_time + flight_time};
        land_event.set_aircraft(ac_id.value());
        m_queue.push(land_event);
    } break;

    default:
        break;
    }
    m_current_time = m_queue.top_time();
}

} // namespace ta
