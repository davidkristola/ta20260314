#include "sequencer.hpp"

#include "gtest/gtest.h"

namespace {
constexpr ta::AircraftType CONDOR = {.m_name                       = "Condor",
                                     .m_cruise_speed               = 100.0,
                                     .m_battery_capacity           = 100.0,
                                     .m_time_to_charge             = 0.95,
                                     .m_energy_used_at_cruise      = 2.0,
                                     .m_passenger_count            = 8U,
                                     .m_fault_probability_per_hour = 0.3};

constexpr ta::AircraftType BUMBLE_BEE = {.m_name                       = "Bumble Bee",
                                         .m_cruise_speed               = 160.0,
                                         .m_battery_capacity           = 400.0,
                                         .m_time_to_charge             = 0.5,
                                         .m_energy_used_at_cruise      = 0.8,
                                         .m_passenger_count            = 2U,
                                         .m_fault_probability_per_hour = 0.75};

constexpr ta::HoursType flight_time(ta::AircraftType ac) {
    return (ac.m_battery_capacity / ac.m_energy_used_at_cruise) / ac.m_cruise_speed;
}

constexpr ta::HoursType FULL_FLIGHT_TIME_CONDOR     = flight_time(CONDOR);
constexpr ta::HoursType FULL_FLIGHT_TIME_BUMBLE_BEE = flight_time(BUMBLE_BEE);

} // namespace

TEST(Sequencer, initial_state) {
    ta::Sequencer uut{{CONDOR, BUMBLE_BEE}, 2, 1, 1.0};
    EXPECT_FALSE(uut.done());
    EXPECT_EQ(2U, uut.aircraft_count());
    EXPECT_EQ(1U, uut.charger_count());
    EXPECT_DOUBLE_EQ(0.0, uut.simulation_time());
    EXPECT_EQ(0U, uut.statistics(BUMBLE_BEE.m_name).value()->total_faults());
    EXPECT_EQ(0U, uut.statistics(CONDOR.m_name).value()->total_faults());
}

TEST(Sequencer, first_step) {
    ta::Sequencer uut{{CONDOR}, 1, 1, 1.0};
    uut.step();
    EXPECT_DOUBLE_EQ(FULL_FLIGHT_TIME_CONDOR, uut.simulation_time());
    EXPECT_EQ(0U, uut.statistics(CONDOR.m_name).value()->total_faults());
}
