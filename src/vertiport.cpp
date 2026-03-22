#include "vertiport.hpp"

#include "common_types.hpp"
#include "event_queue.hpp"
#include "shared_resources.hpp"
#include "statistics.hpp"

namespace ta {

void Vertiport::process_event(const EventType& e, SharedResources& res) {}

void Vertiport::process_landing(const EventType& e, SharedResources& res)
{
    // If there is at least one free charger, post a start charging event to the aircraft
    // TODO(djk): check
    EventType event{e.time(), Cause::start_charging, e.charger(), e.aircraft()};
    res.m_queue.push(event);
}

void Vertiport::start_charging(const EventType& e, SharedResources& res) {}

void Vertiport::complete_charging(const EventType& e, SharedResources& res) {}

void Vertiport::start_waiting(const EventType& e, SharedResources& res) {}

} // namespace ta
