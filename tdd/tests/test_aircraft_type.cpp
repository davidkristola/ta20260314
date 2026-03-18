#include "aircraft_type.hpp"

#include "gtest/gtest.h"

namespace {
constexpr ta::KilowattHoursType CAPACITY    = 300.0;
constexpr ta::KilowattHoursPerMileType WORK = 2.0;
constexpr ta::MilesPerHourType CRUISE_SPEED = 80.0;
constexpr ta::HoursType CHARGE_TIME         = 0.95;
constexpr ta::AircraftType CONDOR           = {.m_name                       = "Condor",
                                               .m_cruise_speed               = CRUISE_SPEED,
                                               .m_battery_capacity           = CAPACITY,
                                               .m_time_to_charge             = CHARGE_TIME,
                                               .m_energy_used_at_cruise      = WORK,
                                               .m_passenger_count            = 8U,
                                               .m_fault_probability_per_hour = 0.13};
} // namespace

TEST(AircraftType, charge_rate) {
    // GIVEN -- a Condor
    // WHEN -- the charge rate is checked
    // THEN -- the charge rate is CAPACITY / CHARGE_TIME
    EXPECT_DOUBLE_EQ(CONDOR.charge_rate(), (CAPACITY / CHARGE_TIME));
}
