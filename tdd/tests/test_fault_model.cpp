#include "fault_model.hpp"

#include "gtest/gtest.h"

// #define DEEP_DIVE
#ifdef DEEP_DIVE
#include <iomanip>
#include <iostream>
#include <map>
#endif

namespace {
double compute_hourly_average(ta::FaultModel& model, ta::ProbabilityPerHourType probability, int samples = 1000000) {
    int faults_within_an_hour = 0;
#ifdef DEEP_DIVE
    double min_time = 1000.0;
    double max_time = 0.0;
    std::map<int, int> histogram;
#endif

    for (int i = 0; i < samples; ++i) {
        const auto time_to_fault = model.time_to_next_fault(probability);
        if (time_to_fault <= 1.0) {
            faults_within_an_hour += 1;
        }

#ifdef DEEP_DIVE
        ++histogram[std::round(time_to_fault * 10.0)];
        if (time_to_fault < min_time)
            min_time = time_to_fault;
        if (time_to_fault > max_time)
            max_time = time_to_fault;
#endif
    }
    const auto answer = static_cast<double>(faults_within_an_hour) / static_cast<double>(samples);
#ifdef DEEP_DIVE
    for (const auto [k, v] : histogram) {
        if (k < 100)
            std::cout << std::setw(5) << (k / 10.0) << ' ' << std::string(v / (samples / 1000), '*') << '\n';
    }
    std::cout << "Faults within the first hour=" << answer << ", min=" << min_time << ", max=" << max_time
              << ", faults=" << faults_within_an_hour << " (out of " << samples << ")\n";
#endif
    return answer;
}
} // namespace

TEST(FaultModel, half) {
    ta::FaultModel uut{};
    const auto answer = compute_hourly_average(uut, 0.5);
    EXPECT_NEAR(0.5, answer, 0.001);
}

TEST(FaultModel, third) {
    ta::FaultModel uut{};
    const auto answer = compute_hourly_average(uut, 0.3);
    EXPECT_NEAR(0.3, answer, 0.001);
}

TEST(FaultModel, three_quarters) {
    ta::FaultModel uut{};
    const auto answer = compute_hourly_average(uut, 0.75);
    EXPECT_NEAR(0.75, answer, 0.001);
}
