#pragma once

#include "common_types.hpp"

#include <random>

namespace ta {

class FaultModel {
    ProbabilityPerHourType m_probability;
    std::mt19937_64 m_generator;

public:
    FaultModel(ProbabilityPerHourType probability) noexcept;
    HoursType time_to_next_fault() noexcept;
};

} // namespace ta
