#include "sequencer.hpp"

#include "gtest/gtest.h"

namespace {
constexpr ta::AircraftType CONDOR = {.m_name                       = "Condor",
                                     .m_cruise_speed               = 100.0,
                                     .m_battery_capacity           = 100.0,
                                     .m_time_to_charge             = 0.2,
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

constexpr ta::HoursType flight_time(ta::AircraftType ac)
{
    return (ac.m_battery_capacity / ac.m_energy_used_at_cruise) / ac.m_cruise_speed;
}
} // namespace

TEST(Sequencer, initial_state)
{
    ta::Sequencer uut{{CONDOR, BUMBLE_BEE}, 2, 1, 1.0};
    EXPECT_FALSE(uut.done());
    EXPECT_EQ(2U, uut.aircraft_count());
    EXPECT_DOUBLE_EQ(0.0, uut.simulation_time());
    EXPECT_EQ(0U, uut.statistics(BUMBLE_BEE.m_name).total_flights());
    EXPECT_EQ(0U, uut.statistics(CONDOR.m_name).total_flights());
}

TEST(Sequencer, first_step)
{
    ta::Sequencer uut{{CONDOR}, 1, 1, 1.0};
    uut.step();
    EXPECT_DOUBLE_EQ(flight_time(CONDOR), uut.simulation_time());
    EXPECT_EQ(0U, uut.statistics(CONDOR.m_name).total_flights());
    const auto aircraft = uut.aircraft(1U);
    EXPECT_EQ(ta::AircraftState::flying, aircraft.state());
}

TEST(Sequencer, take_off_and_land)
{
    ta::Sequencer uut{{CONDOR}, 1, 1, 1.0};
    uut.step();
    uut.step();
    EXPECT_DOUBLE_EQ(flight_time(CONDOR), uut.simulation_time());
    EXPECT_EQ(1U, uut.statistics(CONDOR.m_name).total_flights());
    const auto aircraft = uut.aircraft(1U);
    EXPECT_EQ(ta::AircraftState::idle, aircraft.state());
    EXPECT_DOUBLE_EQ(0.0, aircraft.current_charge());
}

TEST(Sequencer, fly_and_charge)
{
    ta::Sequencer uut{{CONDOR}, 1, 1, 1.0};
    const auto&   aircraft = uut.aircraft(1U);
    uut.step();
    uut.step();
    uut.step();
    EXPECT_EQ(ta::AircraftState::charging, aircraft.state());
    EXPECT_DOUBLE_EQ(flight_time(CONDOR) + CONDOR.m_time_to_charge, uut.simulation_time());
    uut.step();
    EXPECT_EQ(1U, uut.statistics(CONDOR.m_name).total_flights());
    EXPECT_EQ(1U, uut.statistics(CONDOR.m_name).total_charge_sessions());
    EXPECT_EQ(ta::AircraftState::idle, aircraft.state());
    EXPECT_DOUBLE_EQ(1.0, uut.simulation_time());
}

TEST(Sequencer, run_until_done)
{
    ta::Sequencer uut{{CONDOR}, 1, 1, 1.0};
    uut.step();
    uut.step();
    uut.step();
    uut.step();
    uut.step();
    EXPECT_TRUE(uut.done());
}

TEST(Sequencer, dont_blow_up)
{
    ta::Sequencer uut{{CONDOR}, 1, 1, 5.0};
    uut.step();
    uut.step();
    uut.step();
    uut.step();
    uut.step();
    EXPECT_TRUE(uut.done());
    uut.step();
    EXPECT_DOUBLE_EQ(5.0, uut.simulation_time());
}
