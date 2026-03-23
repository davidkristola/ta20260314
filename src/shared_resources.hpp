#pragma once

#include "event_queue.hpp"
#include "fault_model.hpp"
#include "statistics.hpp"

namespace ta {

struct SharedResources {
    StatisticsPool m_statistics;
    EventQueue     m_queue;
    SimEntityId    m_vertiport_id = InvalidSimEntityId;
    FaultModel     m_fault_model;
    bool           disable_faults = false;
};

} // namespace ta
