#pragma once

#include "event_queue.hpp"
#include "statistics.hpp"

namespace ta {

struct SharedResources {
    StatisticsPool m_statistics;
    EventQueue     m_queue;
};

} // namespace ta
