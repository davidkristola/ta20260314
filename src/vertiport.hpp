#pragma once

#include "sim_entity.hpp"

#include <deque>

namespace ta {

class Vertiport : SimEntity {
    ChargerCountType        m_chargers;
    ChargerCountType        m_in_use = 0U;
    std::deque<SimEntityId> m_waiting;

public:
    Vertiport(SimEntityId id, ChargerCountType chargers)
        : SimEntity(id)
        , m_chargers(chargers)
    {}

    virtual void process_event(const EventType& e, SharedResources& res) override;

    ChargerCountType charging() const noexcept { return m_in_use; }
    ChargerCountType capacity() const noexcept { return m_chargers; }
    ChargerCountType waiting() const noexcept { return m_waiting.size(); }

    void process_landing(const EventType& e, SharedResources& res);
    void start_charging(const EventType& e, SharedResources& res);
    void complete_charging(const EventType& e, SharedResources& res);
    void start_waiting(const EventType& e, SharedResources& res);
};

} // namespace ta
