#include "event_type.hpp"

#include "gtest/gtest.h"

TEST(EventType, aircraft)
{
    ta::EventType uut{13.0, ta::Cause::land, 13U};
    EXPECT_DOUBLE_EQ(13.0, uut.time());
    EXPECT_EQ(ta::Cause::land, uut.cause());
    EXPECT_EQ(13U, uut.aircraft());
}

TEST(EventType, charger)
{
    ta::EventType uut{13.0, ta::Cause::start_charging, 13U, 7U};
    EXPECT_DOUBLE_EQ(13.0, uut.time());
    EXPECT_EQ(ta::Cause::start_charging, uut.cause());
    EXPECT_EQ(13U, uut.aircraft());
    EXPECT_EQ(7U, uut.charger());
}
