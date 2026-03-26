#pragma once
/// @file

#include "event_queue.hpp"
#include "fault_model.hpp"
#include "statistics.hpp"

namespace ta {

/// @brief These are resources needed during a simulation run.
/// These are mutable shared resources needed by simulation entities
/// to process events. They are collected here and passed by reference
/// to the entities at event processing time. Other options would be
/// to construct each entity with the resources it needs, or to provide
/// a singleton to serve up the resource when and where it is needed.
//
class SharedResources {
public:
    StatisticsPool m_statistics;
    EventQueue     m_queue;
    SimEntityId    m_vertiport_id = InvalidSimEntityId;
    FaultModel     m_fault_model;
    bool           m_disable_faults = false;

    ///@{
    /// Helper function.
    void        push(EventType event) { m_queue.push(event); }
    Statistics& aircraft_statistics(std::string_view name) { return m_statistics.get(name); }
    SimEntityId vertiport_id() const noexcept { return m_vertiport_id; }
    bool        faults_enabled() const noexcept { return (not m_disable_faults); }
    ///@}
};

} // namespace ta
