#pragma once

#include "common_types.hpp"

namespace ta {

// Forward declare
class EventType;
class SharedResources;

class SimEntity {
protected:
    SimEntityId m_id = InvalidSimEntityId;

public:
    SimEntity() noexcept {}
    explicit SimEntity(SimEntityId id) noexcept
        : m_id(id)
    {}
    virtual ~SimEntity() = default;

    // copy constructor
    SimEntity(const SimEntity& other) noexcept = default;

    // move constructor
    SimEntity(SimEntity&& other) noexcept = default;

    SimEntityId id() const noexcept { return m_id; }

    virtual void process_event(const EventType& e, SharedResources& res) = 0;
};

} // namespace ta
