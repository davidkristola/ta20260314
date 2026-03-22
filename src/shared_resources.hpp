#pragma once

#include "event_queue.hpp"
#include "statistics.hpp"

namespace ta {

struct SharedResources {
    StatisticsPool m_statistics;
    EventQueue     m_queue;
    SimEntityId    m_vertiport_id = InvalidSimEntityId;
};

} // namespace ta
