#pragma once
/// @file

#include "common_types.hpp"

namespace ta {

/// @brief An immutable object for storing and communicating events.
//
class EventType {
    HoursType   m_time = 0.0;
    Cause       m_cause;
    SimEntityId m_subject;
    SimEntityId m_secondary_object;

public:
    /// @brief Constructor.
    /// @param time event time in hours of simulation time.
    /// @param cause the cause of the event.
    /// @param subject the simulation object that will process the event.
    /// @param secondary_object the optional secondary object.
    //
    EventType(HoursType time, Cause cause, SimEntityId subject = InvalidSimEntityId,
              SimEntityId secondary_object = InvalidSimEntityId) noexcept
        : m_time(time)
        , m_cause(cause)
        , m_subject(subject)
        , m_secondary_object(secondary_object)
    {}

    /// @brief Get the time of the event.
    /// @return The time this event occurs.
    //
    HoursType time() const noexcept { return m_time; }

    /// @brief Get the cause (reason, action) of the event.
    /// @return The cause of the event.
    //
    Cause cause() const noexcept { return m_cause; }

    /// @brief Get the subject (doer, effectuator) of the event.
    /// @return The subject of the event.
    //
    SimEntityId subject() const noexcept { return m_subject; }

    /// @brief Get the secondary (indirect) object of the event.
    /// @return The secondary object or \c InvalidSimEntityId
    //
    SimEntityId secondary_object() const noexcept { return m_secondary_object; }
};

} // namespace ta
