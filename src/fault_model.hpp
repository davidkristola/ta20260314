#pragma once

#include "common_types.hpp"

#include <random>

namespace ta {

class FaultModel {
    std::mt19937 m_generator;

public:
    FaultModel() noexcept;
    void          seed(unsigned long seed) noexcept;
    HoursType     time_to_next_fault(ProbabilityPerHourType probability) noexcept;
    unsigned long random_index(unsigned long upper_bound) noexcept;
};

} // namespace ta
