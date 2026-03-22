#pragma once

#include "common_types.hpp"

namespace ta {

class EventType {
    HoursType   m_when = 0.0;
    Cause       m_what;
    SimEntityId m_who;
    SimEntityId m_where;

public:
    EventType(HoursType time, Cause reason, SimEntityId who = InvalidSimEntityId,
              SimEntityId where = InvalidSimEntityId) noexcept
        : m_when(time)
        , m_what(reason) // verb
        , m_who(who)     // subject/agent
        , m_where(where) // object/recipient
    {}
    HoursType   time() const noexcept { return m_when; }
    Cause       cause() const noexcept { return m_what; }
    SimEntityId aircraft() const noexcept { return m_who; }
    SimEntityId target() const noexcept { return m_who; }
    SimEntityId agent() const noexcept { return m_who; }
    SimEntityId charger() const noexcept { return m_where; }
    SimEntityId recipient() const noexcept { return m_where; }
    SimEntityId object() const noexcept { return m_where; }
};

} // namespace ta
