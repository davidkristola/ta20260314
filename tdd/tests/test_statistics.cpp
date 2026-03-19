#include "statistics.hpp"

#include "gtest/gtest.h"

namespace {
constexpr ta::KilowattHoursType CAPACITY    = 300.0;
constexpr ta::KilowattHoursPerMileType WORK = 1.0;
constexpr ta::MilesPerHourType CRUISE_SPEED = 100.0;
constexpr ta::HoursType CHARGE_TIME         = 0.5;
constexpr ta::AircraftType SPARROW          = {.m_name                       = "Sparrow",
                                               .m_cruise_speed               = CRUISE_SPEED,
                                               .m_battery_capacity           = CAPACITY,
                                               .m_time_to_charge             = CHARGE_TIME,
                                               .m_energy_used_at_cruise      = WORK,
                                               .m_passenger_count            = 3U,
                                               .m_fault_probability_per_hour = 0.13};
} // namespace

TEST(Statistics, initial_state) {
    ta::Statistics uut{SPARROW};
    EXPECT_EQ(0U, uut.total_flights());
    EXPECT_DOUBLE_EQ(0.0, uut.total_passenger_miles());
    EXPECT_EQ(0U, uut.total_faults());
}

TEST(Statistics, fault_count) {
    ta::Statistics uut{SPARROW};
    EXPECT_EQ(0U, uut.total_faults());
    uut.record_fault();
    EXPECT_EQ(1U, uut.total_faults());
}

TEST(Statistics, charging_sessions) {
    ta::Statistics uut{SPARROW};
    EXPECT_EQ(0U, uut.total_charge_sessions());
    EXPECT_DOUBLE_EQ(0.0, uut.average_hours_per_charge());

    uut.record_charging_session(0.3);
    EXPECT_EQ(1U, uut.total_charge_sessions());
    EXPECT_DOUBLE_EQ(0.3, uut.average_hours_per_charge());

    uut.record_charging_session(0.3);
    EXPECT_EQ(2U, uut.total_charge_sessions());
    EXPECT_DOUBLE_EQ(0.3, uut.average_hours_per_charge());

    uut.record_charging_session(0.6);
    EXPECT_EQ(3U, uut.total_charge_sessions());
    EXPECT_DOUBLE_EQ((1.2 / 3.0), uut.average_hours_per_charge());
}

TEST(Statistics, flights) {
    ta::Statistics uut{SPARROW};

    EXPECT_EQ(0U, uut.total_flights());
    EXPECT_DOUBLE_EQ(0.0, uut.total_passenger_miles());
    EXPECT_DOUBLE_EQ(0.0, uut.average_hours_per_flight());
    EXPECT_DOUBLE_EQ(0.0, uut.average_miles_per_flight());

    uut.record_flight(1.0, 60.0, 2U);
    EXPECT_EQ(1U, uut.total_flights());
    EXPECT_DOUBLE_EQ(120.0, uut.total_passenger_miles());
    EXPECT_DOUBLE_EQ(1.0, uut.average_hours_per_flight());
    EXPECT_DOUBLE_EQ(60.0, uut.average_miles_per_flight());

    uut.record_flight(3.0, 40.0, 4U);
    EXPECT_EQ(2U, uut.total_flights());
    EXPECT_DOUBLE_EQ(120.0 + (4.0 * 40.0), uut.total_passenger_miles());
    EXPECT_DOUBLE_EQ(2.0, uut.average_hours_per_flight());
    EXPECT_DOUBLE_EQ(50.0, uut.average_miles_per_flight());
}
