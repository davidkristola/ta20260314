#include "event_type.hpp"

#include "gtest/gtest.h"

namespace {
constexpr ta::HoursType   A_TIME      = 13.3;
constexpr ta::SimEntityId AN_AIRCRAFT = 32;
constexpr ta::SimEntityId OBJ_TWO     = 7;
} // namespace

TEST(EventType, subject)
{
    ta::EventType uut{A_TIME, ta::Cause::land, AN_AIRCRAFT};
    EXPECT_DOUBLE_EQ(A_TIME, uut.time());
    EXPECT_EQ(ta::Cause::land, uut.cause());
    EXPECT_EQ(AN_AIRCRAFT, uut.subject());
    EXPECT_EQ(ta::InvalidSimEntityId, uut.secondary_object());
}

TEST(EventType, secondary_object)
{
    ta::EventType uut{A_TIME, ta::Cause::start_charging, AN_AIRCRAFT, OBJ_TWO};
    EXPECT_DOUBLE_EQ(A_TIME, uut.time());
    EXPECT_EQ(ta::Cause::start_charging, uut.cause());
    EXPECT_EQ(AN_AIRCRAFT, uut.subject());
    EXPECT_EQ(OBJ_TWO, uut.secondary_object());
}
