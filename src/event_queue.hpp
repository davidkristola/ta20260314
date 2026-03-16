#pragma once

#include "event_type.hpp"

#include <queue>
#include <vector>

namespace ta {

class EventQueue {
    struct {
        bool operator()(const EventType& l, const EventType& r) const { return l.time() > r.time(); }
    } LowestFirst;
    std::priority_queue<EventType, std::vector<EventType>, decltype(LowestFirst)> m_queue;

public:
    void push(EventType event) noexcept;
    bool empty() const noexcept;
    EventType pop() noexcept;
};

} // namespace ta
