#pragma once
/// @file

#include "event_type.hpp"

#include <queue>
#include <vector>

namespace ta {

/// @brief A time-based priority queue for storing events.
//
class EventQueue {
    /// @brief Operator that defines the sort order (earliest time first) for the queue.
    //
    struct {
        bool operator()(const EventType& l, const EventType& r) const { return l.time() > r.time(); }
    } LowestFirst;

    /// @brief  The queue.
    //
    std::priority_queue<EventType, std::vector<EventType>, decltype(LowestFirst)> m_queue;

    /// @brief  The @c push time limit.
    //
    HoursType m_earliest_allowed_time = 0.0;

public:
    /// @brief Push the @p event onto the time-ordered queue.
    /// @param event
    /// @pre The @p event must have a time that is at or after the current simulation time.
    //
    void push(EventType event);

    /// @brief Check if the queue is empty.
    /// @return @c true if empty, otherwise @c false
    //
    bool empty() const noexcept;

    /// @brief Remove and return the first event on the queue.
    /// @return A previously stored event.
    /// @pre The event queue must not be empty.
    //
    EventType pop();

    /// @brief Get the time of the next event in the queue.
    /// @return The time of the next event in the queue.
    /// @pre The event queue must not be empty.
    //
    HoursType top_time() const;
};

} // namespace ta
