#include "event_type.hpp"

#include "gtest/gtest.h"

TEST(EventType, aircraft) {
    ta::EventType uut{1, ta::Cause::land, 13.0};
    const auto null_id = uut.aircraft();
    EXPECT_FALSE(null_id);
    uut.set_aircraft(13U);
    const auto real_id = uut.aircraft();
    EXPECT_TRUE(real_id);
    EXPECT_EQ(real_id.value_or(20U), 13U);
}

TEST(EventType, charger) {
    ta::EventType uut{1, ta::Cause::start_charging, 13.0};
    const auto null_id = uut.charger();
    EXPECT_FALSE(null_id);
    uut.set_charger(13U);
    const auto real_id = uut.charger();
    EXPECT_TRUE(real_id);
    EXPECT_EQ(real_id.value_or(20U), 13U);
}
