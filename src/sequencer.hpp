#pragma once

#include "aircraft.hpp"
#include "common_types.hpp"
#include "event_queue.hpp"
#include "statistics.hpp"

#include <array>
#include <optional>
#include <vector>

namespace ta {

class Charger {
    ChargerId m_id;
    bool m_available             = true;
    AircraftId m_last_to_connect = 0U;

public:
    Charger(ChargerId id) noexcept : m_id(id) {}
    void start(AircraftId id) noexcept {
        m_available       = false;
        m_last_to_connect = id;
    }
    void stop() noexcept { m_available = true; }
    bool available() const noexcept { return m_available; }
};

class Sequencer {
    std::vector<AircraftType> m_aircraft_types;
    std::vector<Aircraft> m_aircraft;
    std::vector<Statistics> m_statistics;
    std::vector<Charger> m_chargers;
    int m_next_event_id = 0;
    EventQueue m_queue;
    HoursType m_current_time = 0.0;

public:
    Sequencer(std::vector<AircraftType> aircraft_types, AircraftCountType ac, ChargerCountType cc,
              HoursType run_time) noexcept;

    bool done() const noexcept;
    HoursType simulation_time() const noexcept;
    AircraftCountType aircraft_count() const noexcept { return m_aircraft.size(); }
    ChargerCountType charger_count() const noexcept { return m_chargers.size(); }
    const Aircraft& aircraft(AircraftCountType index) const { return m_aircraft[index]; }
    std::optional<const Statistics*> statistics(std::string_view type_name) const;

    void step() noexcept;
};

} // namespace ta
