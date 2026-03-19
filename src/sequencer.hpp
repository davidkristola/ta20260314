#pragma once

#include "aircraft.hpp"
#include "common_types.hpp"
#include "event_queue.hpp"
#include "statistics.hpp"

#include <array>
#include <optional>
#include <vector>

namespace ta {

class Sequencer {
    std::vector<AircraftType> m_aircraft_types;
    AircraftCountType m_aircraft_count;
    ChargerCountType m_charger_count;
    std::vector<Aircraft> m_aircraft;
    std::vector<Statistics> m_statistics;

public:
    Sequencer(std::vector<AircraftType> aircraft_types, AircraftCountType ac, ChargerCountType cc,
              HoursType run_time) noexcept;

    bool done() const noexcept;
    HoursType simulation_time() const noexcept;
    AircraftCountType aircraft_count() const noexcept { return m_aircraft.size(); }
    const Aircraft& aircraft(AircraftCountType index) const { return m_aircraft[index]; }
    std::optional<const Statistics*> statistics(std::string_view type_name) const;

    void step() noexcept;
};

} // namespace ta
