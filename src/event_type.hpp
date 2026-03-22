#pragma once

#include "common_types.hpp"

namespace ta {

class EventType {
    HoursType   m_when = 0.0;
    Cause       m_what;
    SimEntityId m_who;
    SimEntityId m_where;

public:
    EventType(HoursType time, Cause reason, SimEntityId who = 0U, SimEntityId where = 0U) noexcept
        : m_when(time)
        , m_what(reason) // verb
        , m_who(who)     // subject
        , m_where(where) // object
    {}
    HoursType   time() const noexcept { return m_when; }
    Cause       cause() const noexcept { return m_what; }
    SimEntityId aircraft() const noexcept { return m_who; } // TODO(djk): rename to who or target
    SimEntityId charger() const noexcept { return m_where; }
};

} // namespace ta
