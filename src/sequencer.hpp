#pragma once

#include "aircraft.hpp"
#include "common_types.hpp"
#include "event_queue.hpp"
#include "shared_resources.hpp"
#include "statistics.hpp"
#include "vertiport.hpp"

#include <map>
#include <vector>

namespace ta {

struct Configuration {
    std::vector<ta::AircraftType> aircraft_types;
    AircraftCountType             aircraft_count = 20;
    ChargerCountType              charger_count  = 3;
    HoursType                     run_time       = 3.0;
    SeedType                      seed           = 0U;
    bool                          disable_faults = false;
};

class Sequencer {
    std::vector<AircraftType>       m_aircraft_types;
    std::map<SimEntityId, Aircraft> m_aircraft;
    HoursType                       m_current_time = 0.0;
    SharedResources                 m_shared;
    bool                            m_done = false;
    Vertiport                       m_vertiport;

public:
    Sequencer(Configuration configuration) noexcept;

    bool              done() const noexcept { return m_done; }
    HoursType         simulation_time() const noexcept { return m_current_time; }
    AircraftCountType aircraft_count() const noexcept { return m_aircraft.size(); }
    Aircraft&         aircraft(SimEntityId id) { return m_aircraft[id]; }

    SharedResources& shared_resources() noexcept { return m_shared; }

    // Return a *copy* of the requested statistics at this point in time.
    Statistics statistics(std::string_view type_name);

    void step() noexcept;

private:
    const AircraftType& pick_type(SimEntityId id) noexcept;
};

} // namespace ta
