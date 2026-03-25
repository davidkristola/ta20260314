/// @file
#include "event_queue.hpp"

#include <iostream>
#include <stdexcept>

namespace {
/// Convert a @c Cause enumeration into a C-string.
const char* image(ta::Cause c)
{
    switch (c) {
    case ta::Cause::complete_charging:
        return "complete_charging";
    case ta::Cause::end_of_simulation:
        return "end_of_simulation";
    case ta::Cause::experience_fault:
        return "experience_fault";
    case ta::Cause::land:
        return "land";
    case ta::Cause::start_charging:
        return "start_charging";
    case ta::Cause::take_off:
        return "take_off";
    default:
        break;
    }
    return "unknown cause";
}

// TODO: Logging would be a useful addition.
void output_event(std::string dir, ta::EventType event)
{
#ifdef DEBUG
    std::cout << "Event Queue " << dir << " at " << event.time() << " " << image(event.cause())
              << " subject=" << event.agent() << " object=" << event.object() << "\n";
#endif
}
} // namespace

namespace ta {

void EventQueue::push(EventType event)
{
    if (event.time() < m_earliest_allowed_time) {
        throw std::runtime_error("Cannot push event into the past!");
    }
    output_event("push", event);
    m_queue.push(event);
}

bool EventQueue::empty() const noexcept { return m_queue.empty(); }

EventType EventQueue::pop()
{
    if (empty()) {
        throw std::runtime_error("Cannot pop from an empty queue!");
    }
    const auto event        = m_queue.top();
    m_earliest_allowed_time = event.time();
    output_event("pop", event);
    m_queue.pop();
    return event;
}

HoursType EventQueue::top_time() const
{
    if (empty()) {
        throw std::runtime_error("Cannot peek at the next event time from an empty queue!");
    }
    const auto event = m_queue.top();
    return event.time();
}

} // namespace ta
