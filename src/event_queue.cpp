#include "event_queue.hpp"

#include <iostream>

namespace {
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
void output_event(std::string dir, ta::EventType event)
{
#ifdef DEBUG
    std::cout << "Event Queue " << dir << " at " << event.time() << " " << image(event.cause())
              << " subject=" << event.agent() << " object=" << event.object() << "\n";
#endif
}
} // namespace

namespace ta {

void EventQueue::push(EventType event) noexcept
{
    output_event("push", event);
    m_queue.push(event);
}

bool EventQueue::empty() const noexcept { return m_queue.empty(); }

EventType EventQueue::pop() noexcept
{
    const auto event = m_queue.top();
    output_event("pop", event);
    m_queue.pop();
    return event;
}

HoursType EventQueue::top_time() const noexcept
{
    const auto event = m_queue.top();
    return event.time();
}

} // namespace ta
