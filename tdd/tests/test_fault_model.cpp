#include "fault_model.hpp"

#include "gtest/gtest.h"

#include <iomanip>
#include <iostream>
#include <map>

namespace {
double compute_hourly_average(ta::FaultModel& model, int samples = 1000000) {
    int faults_within_an_hour = 0;
    double min_time           = 1000.0;
    double max_time           = 0.0;
    std::map<int, int> histogram;

    for (int i = 0; i < samples; ++i) {
        const auto time_to_fault = model.time_to_next_fault();
        if (time_to_fault <= 1.0) {
            faults_within_an_hour += 1;
        }

        ++histogram[std::round(time_to_fault * 10.0)];
        if (time_to_fault < min_time)
            min_time = time_to_fault;
        if (time_to_fault > max_time)
            max_time = time_to_fault;
    }
    const auto answer = static_cast<double>(faults_within_an_hour) / static_cast<double>(samples);
    for (const auto [k, v] : histogram) {
        if (k < 100)
            std::cout << std::setw(5) << (k / 10.0) << ' ' << std::string(v / 1000, '*') << '\n';
    }
    std::cout << "Faults within the first hour=" << answer << ", min=" << min_time << ", max=" << max_time
              << ", faults=" << faults_within_an_hour << " (out of " << samples << ")\n";
    return answer;
}
} // namespace

TEST(FaultModel, half) {
    ta::FaultModel uut{0.5};
    const auto answer = compute_hourly_average(uut);
    EXPECT_NEAR(0.5, answer, 0.001);
}

TEST(FaultModel, third) {
    ta::FaultModel uut{0.3};
    const auto answer = compute_hourly_average(uut);
    EXPECT_NEAR(0.3, answer, 0.001);
}
