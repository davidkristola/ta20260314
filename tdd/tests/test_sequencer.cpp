#include "sequencer.hpp"

#include "gtest/gtest.h"

#include <cmath>

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
constexpr ta::HoursType charge_time(ta::AircraftType ac) { return ac.m_time_to_charge; }
} // namespace

TEST(Sequencer, initial_state)
{
    ta::Configuration configuration{.aircraft_types = {CONDOR, BUMBLE_BEE},
                                    .aircraft_count = 2,
                                    .charger_count  = 1,
                                    .run_time       = 1.0,
                                    .disable_faults = true};
    ta::Sequencer     uut{configuration};
    EXPECT_FALSE(uut.done());
    EXPECT_EQ(2U, uut.aircraft_count());
    EXPECT_DOUBLE_EQ(0.0, uut.simulation_time());
    EXPECT_EQ(0U, uut.statistics(BUMBLE_BEE.m_name).total_flights());
    EXPECT_EQ(0U, uut.statistics(CONDOR.m_name).total_flights());
}

TEST(Sequencer, first_step)
{
    ta::Configuration configuration{
        .aircraft_types = {CONDOR}, .aircraft_count = 1, .charger_count = 1, .run_time = 1.0, .disable_faults = true};
    ta::Sequencer uut{configuration};
    uut.shared_resources().m_disable_faults = true;
    uut.step();
    EXPECT_DOUBLE_EQ(flight_time(CONDOR), uut.simulation_time());
    EXPECT_EQ(0U, uut.statistics(CONDOR.m_name).total_flights());
    const auto aircraft = uut.aircraft(1U);
    EXPECT_EQ(ta::AircraftState::flying, aircraft.state());
}

TEST(Sequencer, take_off_and_land)
{
    ta::Configuration configuration{
        .aircraft_types = {CONDOR}, .aircraft_count = 1, .charger_count = 1, .run_time = 1.0, .disable_faults = true};
    ta::Sequencer uut{configuration};
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
    ta::Configuration configuration{
        .aircraft_types = {CONDOR}, .aircraft_count = 1, .charger_count = 1, .run_time = 1.0, .disable_faults = true};
    ta::Sequencer uut{configuration};
    const auto&   aircraft = uut.aircraft(1U);
    uut.step(); // takeoff event to aircraft
    uut.step(); // landing event to aircraft
    uut.step(); // landing event to vertiport
    uut.step(); // charging event to aircraft
    EXPECT_EQ(ta::AircraftState::charging, aircraft.state());
    EXPECT_DOUBLE_EQ(flight_time(CONDOR) + CONDOR.m_time_to_charge, uut.simulation_time());
    uut.step(); // done charging event to aircraft
    EXPECT_EQ(ta::AircraftState::idle, aircraft.state());
    uut.step(); // take off again
    uut.step(); // end of simulation
    EXPECT_EQ(1U, uut.statistics(CONDOR.m_name).total_flights());
    EXPECT_EQ(1U, uut.statistics(CONDOR.m_name).total_charge_sessions());
    EXPECT_EQ(ta::AircraftState::flying, aircraft.state());
    EXPECT_DOUBLE_EQ(1.0, uut.simulation_time());
}

TEST(Sequencer, run_until_done)
{
    ta::Configuration configuration{
        .aircraft_types = {CONDOR}, .aircraft_count = 1, .charger_count = 1, .run_time = 1.0, .disable_faults = true};
    ta::Sequencer uut{configuration};
    uut.step(); // takeoff event to aircraft
    uut.step(); // landing event to aircraft
    uut.step(); // landing event to vertiport
    uut.step(); // charging event to aircraft
    uut.step(); // done charging event to aircraft
    uut.step(); // take off again
    uut.step(); // end of simulation
    EXPECT_TRUE(uut.done());
}

TEST(Sequencer, dont_blow_up)
{
    ta::Configuration configuration{
        .aircraft_types = {CONDOR}, .aircraft_count = 1, .charger_count = 1, .run_time = 1.0, .disable_faults = true};
    ta::Sequencer uut{configuration};
    uut.step(); // takeoff event to aircraft
    uut.step(); // landing event to aircraft
    uut.step(); // landing event to vertiport
    uut.step(); // charging event to aircraft
    uut.step(); // done charging event to aircraft
    uut.step(); // take off again
    uut.step(); // end of simulation
    EXPECT_TRUE(uut.done());
    uut.step();
    EXPECT_DOUBLE_EQ(1.0, uut.simulation_time());
}

TEST(Sequencer, bigger_run)
{
    constexpr ta::HoursType SIM_TIME = 5.0;
    ta::Configuration       configuration{.aircraft_types = {CONDOR, BUMBLE_BEE},
                                          .aircraft_count = 2,
                                          .charger_count  = 2,
                                          .run_time       = SIM_TIME,
                                          .disable_faults = true};
    ta::Sequencer           uut{configuration};
    while (not uut.done()) {
        uut.step();
    }
    EXPECT_DOUBLE_EQ(SIM_TIME, uut.simulation_time());

    const auto condor_cycles =
        static_cast<unsigned int>(std::floor(SIM_TIME / (flight_time(CONDOR) + charge_time(CONDOR))));
    EXPECT_EQ(condor_cycles, uut.statistics(CONDOR.m_name).total_flights());
    EXPECT_EQ(condor_cycles, uut.statistics(CONDOR.m_name).total_charge_sessions());

    const auto bumble_cycles =
        static_cast<unsigned int>(std::floor(SIM_TIME / (flight_time(BUMBLE_BEE) + charge_time(BUMBLE_BEE))));
    EXPECT_EQ(bumble_cycles, uut.statistics(BUMBLE_BEE.m_name).total_flights());
    EXPECT_EQ(bumble_cycles, uut.statistics(BUMBLE_BEE.m_name).total_charge_sessions());
}
