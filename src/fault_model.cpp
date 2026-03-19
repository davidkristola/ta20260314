#include "fault_model.hpp"

#include <cmath>

namespace ta {

FaultModel::FaultModel(ProbabilityPerHourType probability) noexcept : m_probability(probability), m_generator() {}

HoursType FaultModel::time_to_next_fault() noexcept {
    // const auto probability_per_second = m_probability / 3600.0;
    // std::uniform_real_distribution<> distribution(0.0, 1.0);
    // for (auto s = 0U; s < 10000000U; ++s) {
    //     const auto p = distribution(m_generator);
    //     if (p <= probability_per_second) {
    //         return static_cast<HoursType>(s) / 3600.0;
    //     }
    // }
    // return 0.0;

    constexpr HoursType HOUR      = 1.0;
    const auto mean_time_to_fault = HOUR / m_probability;
    // const auto mean_time_to_fault = HOUR / (-std::log(1.0 - m_probability));

    // std::normal_distribution distribution(mean_time_to_fault, 0.90);
    // std::lognormal_distribution distribution(mean_time_to_fault, 2.0);
    // std::exponential_distribution distribution(mean_time_to_fault);
    // std::cauchy_distribution distribution(mean_time_to_fault, 0.1);

    std::uniform_real_distribution distribution(0.0, mean_time_to_fault);

    return distribution(m_generator);
}

} // namespace ta
