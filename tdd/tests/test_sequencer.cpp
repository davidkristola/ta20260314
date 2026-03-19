#include "sequencer.hpp"

#include "gtest/gtest.h"

namespace {
constexpr ta::AircraftType CONDOR = {.m_name                       = "Condor",
                                     .m_cruise_speed               = 80.0,
                                     .m_battery_capacity           = 300.0,
                                     .m_time_to_charge             = 0.95,
                                     .m_energy_used_at_cruise      = 2.0,
                                     .m_passenger_count            = 8U,
                                     .m_fault_probability_per_hour = 0.3};

constexpr ta::AircraftType BUMBLE_BEE = {.m_name                       = "Bumble Bee",
                                         .m_cruise_speed               = 100.0,
                                         .m_battery_capacity           = 500.0,
                                         .m_time_to_charge             = 0.5,
                                         .m_energy_used_at_cruise      = 0.8,
                                         .m_passenger_count            = 2U,
                                         .m_fault_probability_per_hour = 0.75};
} // namespace

TEST(Sequencer, initial_state) {
    ta::Sequencer uut{{CONDOR, BUMBLE_BEE}, 2, 1, 3.0};
    EXPECT_FALSE(uut.done());
    EXPECT_EQ(2U, uut.aircraft_count());
    EXPECT_DOUBLE_EQ(0.0, uut.simulation_time());
    EXPECT_EQ(0U, uut.statistics(BUMBLE_BEE.m_name).value()->total_faults());
}
