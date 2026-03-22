#include "vertiport.hpp"

#include "shared_resources.hpp"

#include "gtest/gtest.h"

namespace {
constexpr ta::SimEntityId VERTIPORT_ID = 88U;
constexpr ta::SimEntityId AC_1_ID      = 1U;
constexpr ta::SimEntityId AC_2_ID      = 2U;
constexpr ta::SimEntityId AC_3_ID      = 3U;
constexpr ta::SimEntityId AC_4_ID      = 4U;
} // namespace

TEST(Vertiport, init)
{
    ta::Vertiport uut{VERTIPORT_ID, 3U};
    EXPECT_EQ(0U, uut.charging());
    EXPECT_EQ(3U, uut.capacity());
    EXPECT_EQ(0U, uut.waiting());
}

TEST(Vertiport, process_landing)
{
    ta::Vertiport       uut{VERTIPORT_ID, 3U};
    ta::SharedResources sr{};

    // GIVEN -- a landing event
    ta::EventType landing{0.1, ta::Cause::land, AC_1_ID, VERTIPORT_ID};

    // WHEN -- AC_1 lands
    uut.process_landing(landing, sr);

    // THEN -- there are free chargers so a start charging event is posted
    const auto event = sr.m_queue.pop();
    EXPECT_DOUBLE_EQ(0.1, event.time());
    EXPECT_EQ(AC_1_ID, event.aircraft());
    EXPECT_EQ(ta::Cause::start_charging, event.cause());
    EXPECT_EQ(VERTIPORT_ID, event.charger());

    EXPECT_EQ(0U, uut.charging());
    EXPECT_EQ(0U, uut.waiting());
}

TEST(Vertiport, start_charging)
{
    ta::Vertiport       uut{VERTIPORT_ID, 3U};
    ta::SharedResources sr{};

    // GIVEN -- a charging event
    ta::EventType charging{0.1, ta::Cause::start_charging, AC_1_ID, VERTIPORT_ID};

    // WHEN -- AC_1 starts charging
    uut.start_charging(charging, sr);

    // THEN -- one is charging (no new events are posted)
    EXPECT_TRUE(sr.m_queue.empty());

    EXPECT_EQ(1U, uut.charging());
    EXPECT_EQ(0U, uut.waiting());
}
