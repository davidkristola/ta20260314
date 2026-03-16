#pragma once

namespace ta {

using HoursType = double;

enum class Cause { end_of_simulation, take_off, land, start_charging, complete_charging, experience_fault };

class EventType {
    int m_id         = 0;
    HoursType m_when = 0.0;
    Cause m_what;

public:
    EventType(int id, Cause reason, HoursType time) : m_id(id), m_when(time), m_what(reason) {}
    HoursType time() const noexcept { return m_when; }
    int id() const noexcept { return m_id; }
    Cause cause() const noexcept { return m_what; }
};

} // namespace ta
