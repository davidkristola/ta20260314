#pragma once

#include <optional>

namespace ta {

using AircraftId = unsigned long int;
using ChargerId  = unsigned long int;

using HoursType = double;

enum class Cause { end_of_simulation, take_off, land, start_charging, complete_charging, experience_fault };

class EventType {
    int m_id         = 0;
    HoursType m_when = 0.0;
    Cause m_what;
    std::optional<AircraftId> m_who;
    std::optional<ChargerId> m_where;

public:
    EventType(int id, Cause reason, HoursType time) noexcept : m_id(id), m_when(time), m_what(reason) {}
    HoursType time() const noexcept { return m_when; }
    int id() const noexcept { return m_id; }
    Cause cause() const noexcept { return m_what; }
    void set_aircraft(AircraftId id) noexcept { m_who = id; }
    void set_charger(ChargerId id) noexcept { m_where = id; }
    std::optional<AircraftId> aircraft() const noexcept { return m_who; }
    std::optional<ChargerId> charger() const noexcept { return m_where; }
};

} // namespace ta
