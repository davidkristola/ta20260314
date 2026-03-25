#pragma once
/// @file

#include "common_types.hpp"

namespace ta {

// Forward declare
class EventType;
class SharedResources;

/// Base class for simulation entities.
/// This class isn't completely abstract because it holds an identifier for
/// the simulated entity.
class SimEntity {
    SimEntityId m_id = InvalidSimEntityId;

protected:
    /// Main constructor.
    explicit SimEntity(SimEntityId id) noexcept
        : m_id(id)
    {}

public:
    /// @brief Default constructor.
    SimEntity() noexcept {}

    /// Virtual destructor.
    virtual ~SimEntity() = default;

    /// Copy constructor.
    SimEntity(const SimEntity& other) noexcept = default;

    /// Move constructor.
    SimEntity(SimEntity&& other) noexcept = default;

    /// @brief Simulation entity identity.
    /// @return Unique ID
    SimEntityId id() const noexcept { return m_id; }

    /// @brief Virtual abstract function to deliver an @p event to the simulated entity.
    /// @param event
    /// @param resources
    virtual void process_event(const EventType& event, SharedResources& resources) = 0;
};

} // namespace ta
