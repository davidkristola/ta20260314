#include "fault_model.hpp"

namespace ta {

FaultModel::FaultModel() noexcept
    : m_generator()
{}

void FaultModel::seed(unsigned long seed) noexcept { m_generator.seed(seed); }

HoursType FaultModel::time_to_next_fault(ProbabilityPerHourType probability) noexcept
{
    constexpr HoursType            HOUR               = 1.0;
    const auto                     mean_time_to_fault = HOUR / probability;
    std::uniform_real_distribution distribution(0.0, mean_time_to_fault);
    return distribution(m_generator);
}

unsigned long FaultModel::random_index(unsigned long upper_bound) noexcept
{
    std::uniform_int_distribution<> distribution(0, upper_bound);
    return distribution(m_generator);
}

} // namespace ta
