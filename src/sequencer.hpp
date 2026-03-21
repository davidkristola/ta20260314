#pragma once

#include "aircraft.hpp"
#include "common_types.hpp"
#include "event_queue.hpp"
#include "shared_resources.hpp"
#include "statistics.hpp"

#include <array>
#include <map>
#include <optional>
#include <vector>

namespace ta {

class Charger {
    SimEntityId m_id;
    bool        m_available       = true;
    SimEntityId m_last_to_connect = 0U;

public:
    Charger(SimEntityId id) noexcept
        : m_id(id)
    {}
    void start(SimEntityId id) noexcept
    {
        m_available       = false;
        m_last_to_connect = id;
    }
    void stop() noexcept { m_available = true; }
    bool available() const noexcept { return m_available; }
};

class Sequencer {
    std::vector<AircraftType>       m_aircraft_types;
    std::map<SimEntityId, Aircraft> m_aircraft;
    //    StatisticsPool                  m_statistics;
    std::vector<Charger> m_chargers;
    int                  m_next_event_id = 0;
    //    EventQueue                      m_queue;
    HoursType       m_current_time = 0.0;
    SharedResources m_shared;

public:
    Sequencer(std::vector<AircraftType> aircraft_types, AircraftCountType ac, ChargerCountType cc,
              HoursType run_time) noexcept;

    bool              done() const noexcept;
    HoursType         simulation_time() const noexcept;
    AircraftCountType aircraft_count() const noexcept { return m_aircraft.size(); }
    ChargerCountType  charger_count() const noexcept { return m_chargers.size(); }
    Aircraft&         aircraft(SimEntityId id) { return m_aircraft[id]; }

    // Return a *copy* of the requested statistics at this point in time.
    Statistics statistics(std::string_view type_name);

    void step() noexcept;

private:
    const AircraftType& pick_type(SimEntityId id) const noexcept;
};

} // namespace ta
