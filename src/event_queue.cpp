#include "event_queue.hpp"

namespace ta {

void EventQueue::push(EventType event) noexcept { m_queue.push(event); }

bool EventQueue::empty() const noexcept { return m_queue.empty(); }

EventType EventQueue::pop() noexcept {
    const auto event = m_queue.top();
    m_queue.pop();
    return event;
}

HoursType EventQueue::top_time() const noexcept {
    const auto event = m_queue.top();
    return event.time();
}

} // namespace ta
