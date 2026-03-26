/// @file
#include "fault_model.hpp"

namespace ta {

FaultModel::FaultModel() noexcept
    : m_generator()
{}

void FaultModel::seed(unsigned int seed) noexcept { m_generator.seed(seed); }

/// @brief Compute a time to the next fault.
/// Compute a time to the next fault given an aircraft's probability-of-failure-per-hour
/// (of flight). This model works to generate the correct time (within a few orders of magnitude),
/// but is not very realistic.
/// @param probability
/// @return Time to a fault.
//
HoursType FaultModel::time_to_next_fault(ProbabilityPerHourType probability) noexcept
{
    constexpr HoursType            HOUR               = 1.0;
    const auto                     mean_time_to_fault = HOUR / probability;
    std::uniform_real_distribution distribution(0.0, mean_time_to_fault);
    return distribution(m_generator);
}

std::size_t FaultModel::random_index(int upper_bound) noexcept
{
    std::uniform_int_distribution<> distribution(0, upper_bound);
    return static_cast<std::size_t>(distribution(m_generator));
}

} // namespace ta
