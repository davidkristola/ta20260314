#include "aircraft.hpp"

#include "shared_resources.hpp"

#include "gtest/gtest.h"

namespace {
constexpr ta::KilowattHoursType        ENERGY           = 500.0;
constexpr ta::KilowattHoursPerMileType WORK             = 1.0;
constexpr ta::MilesPerHourType         CRUISE_SPEED     = 100.0;
constexpr ta::HoursType                CHARGE_TIME      = 0.5;
constexpr ta::AircraftType             BUMBLE_BEE       = {.m_name                       = "Bumble Bee",
                                                           .m_cruise_speed               = CRUISE_SPEED,
                                                           .m_battery_capacity           = ENERGY,
                                                           .m_time_to_charge             = CHARGE_TIME,
                                                           .m_energy_used_at_cruise      = WORK,
                                                           .m_passenger_count            = 2U,
                                                           .m_fault_probability_per_hour = 0.13};
constexpr ta::HoursType                FULL_FLIGHT_TIME = (ENERGY / WORK) / CRUISE_SPEED;
constexpr ta::SimEntityId              VERTIPORT_ID     = 88U;
} // namespace

TEST(Aircraft, init)
{
    ta::Aircraft uut{BUMBLE_BEE, 32U};
    EXPECT_EQ(32U, uut.id());
    EXPECT_DOUBLE_EQ(uut.current_charge(), ENERGY);
    EXPECT_DOUBLE_EQ(uut.charge_time(), 0.0);
    EXPECT_EQ(ta::AircraftState::idle, uut.state());
}

TEST(Aircraft, flight_distance)
{
    ta::Aircraft uut{BUMBLE_BEE, 32U};
    EXPECT_DOUBLE_EQ(uut.flight_distance(), ENERGY / WORK);
}

TEST(Aircraft, flight_time)
{
    ta::Aircraft uut{BUMBLE_BEE, 32U};
    EXPECT_DOUBLE_EQ(uut.flight_time(), FULL_FLIGHT_TIME);
}

TEST(Aircraft, charge_time)
{
    ta::Aircraft uut{BUMBLE_BEE, 32U};
    // We are fully charged by default
    EXPECT_DOUBLE_EQ(uut.charge_time(), 0.0);
}

TEST(Aircraft, charge_time_empty)
{
    ta::Aircraft        uut{BUMBLE_BEE, 32U};
    ta::SharedResources sr{};
    uut.fly_for(FULL_FLIGHT_TIME, sr);
    EXPECT_DOUBLE_EQ(uut.current_charge(), 0.0);
    EXPECT_DOUBLE_EQ(uut.charge_time(), CHARGE_TIME);
}

TEST(Aircraft, charge_time_half_empty)
{
    ta::Aircraft        uut{BUMBLE_BEE, 32U};
    ta::SharedResources sr{};
    uut.fly_for(FULL_FLIGHT_TIME / 2.0, sr);
    EXPECT_DOUBLE_EQ(uut.current_charge(), (ENERGY / 2.0));
    EXPECT_DOUBLE_EQ(uut.charge_time(), (CHARGE_TIME / 2.0));
}

TEST(Aircraft, cumulative_flight_time)
{
    ta::Aircraft        uut{BUMBLE_BEE, 32U};
    ta::SharedResources sr{};
    uut.fly_for(FULL_FLIGHT_TIME / 2.0, sr);
    uut.fly_for(FULL_FLIGHT_TIME / 2.0, sr);
    EXPECT_DOUBLE_EQ(uut.current_charge(), 0.0);
    EXPECT_DOUBLE_EQ(uut.charge_time(), CHARGE_TIME);
}

TEST(Aircraft, charging_half)
{
    ta::Aircraft        uut{BUMBLE_BEE, 32U};
    ta::SharedResources sr{};
    uut.fly_for(FULL_FLIGHT_TIME, sr);
    EXPECT_DOUBLE_EQ(uut.current_charge(), 0.0);
    uut.charge_for(CHARGE_TIME / 2.0, sr);

    EXPECT_DOUBLE_EQ(uut.charge_time(), CHARGE_TIME / 2.0);
}

TEST(Aircraft, take_off)
{
    ta::Aircraft        uut{BUMBLE_BEE, 32U};
    ta::SharedResources sr{};
    sr.disable_faults = true;
    ta::EventType takeoff_event{0.0, ta::Cause::take_off, 32U, VERTIPORT_ID};
    sr.m_vertiport_id = VERTIPORT_ID;
    uut.process_event(takeoff_event, sr);
    EXPECT_EQ(ta::AircraftState::flying, uut.state());
    ASSERT_FALSE(sr.m_queue.empty());
    const auto event = sr.m_queue.pop();
    EXPECT_DOUBLE_EQ(FULL_FLIGHT_TIME, event.time());
    EXPECT_EQ(32U, event.aircraft());
    EXPECT_EQ(ta::Cause::land, event.cause());
    EXPECT_EQ(VERTIPORT_ID, event.recipient());
}

TEST(Aircraft, land)
{
    ta::Aircraft        uut{BUMBLE_BEE, 32U};
    ta::SharedResources sr{};
    sr.disable_faults = true;
    ta::EventType event{FULL_FLIGHT_TIME, ta::Cause::land, 32U, VERTIPORT_ID};
    sr.m_vertiport_id = VERTIPORT_ID;
    uut.process_event(event, sr);
    EXPECT_EQ(ta::AircraftState::idle, uut.state());
    ASSERT_FALSE(sr.m_queue.empty());
    event = sr.m_queue.pop();
    EXPECT_DOUBLE_EQ(FULL_FLIGHT_TIME, event.time());
    EXPECT_EQ(VERTIPORT_ID, event.target());
    EXPECT_EQ(ta::Cause::land, event.cause());
    EXPECT_EQ(32, event.object());
}
