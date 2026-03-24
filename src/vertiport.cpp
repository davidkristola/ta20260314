#include "vertiport.hpp"

#include "common_types.hpp"
#include "event_queue.hpp"
#include "shared_resources.hpp"
#include "statistics.hpp"

namespace ta {

void Vertiport::process_event(const EventType& e, SharedResources& res)
{
    switch (e.cause()) {
    case Cause::land: {
        process_landing(e, res);
    } break;
    case Cause::start_charging: {
        start_charging(e, res);
    } break;
    case Cause::complete_charging: {
        complete_charging(e, res);
    } break;

    default:
        break;
    }
}

void Vertiport::process_landing(const EventType& e, SharedResources& res)
{
    const auto aircraft_id = e.secondary_object();
    if (charging() == capacity()) {
        m_waiting.emplace_back(aircraft_id);
    } else {
        // If there is at least one free charger, post a start charging event to the aircraft
        EventType event{e.time(), Cause::start_charging, aircraft_id, id()};
        res.m_queue.push(event);
    }
}

void Vertiport::start_charging(const EventType& e, SharedResources& res) { m_in_use += 1; }

void Vertiport::complete_charging(const EventType& e, SharedResources& res)
{
    m_in_use -= 1;
    if (not m_waiting.empty()) {
        const SimEntityId aircraft_id = m_waiting.front();
        m_waiting.pop_front();
        EventType event{e.time(), Cause::start_charging, aircraft_id, id()};
        res.m_queue.push(event);
    }
}

} // namespace ta
