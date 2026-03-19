#include "sequencer.hpp"

namespace ta {
Sequencer::Sequencer(std::vector<AircraftType> aircraft_types, AircraftCountType ac, ChargerCountType cc,
                     HoursType run_time) noexcept
    : m_aircraft_types(aircraft_types), m_aircraft_count(ac), m_charger_count(cc) {
    for (AircraftId id = 1U; id <= ac; ++id) {
        m_aircraft.push_back({m_aircraft_types[1], id});
    }
    for (const auto& t : m_aircraft_types) {
        m_statistics.push_back(Statistics{t});
    }
}

bool Sequencer::done() const noexcept { return false; }
HoursType Sequencer::simulation_time() const noexcept { return HoursType(); }

std::optional<const Statistics*> Sequencer::statistics(std::string_view type_name) const {
    for (const auto& s : m_statistics) {
        if (s.name() == type_name) {
            return &s;
        }
    }
    return std::optional<const Statistics*>{};
}
void Sequencer::step() noexcept {}
} // namespace ta
