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

TEST(Vertiport, process_landing_nominal)
{
    ta::Vertiport       uut{VERTIPORT_ID, 3U};
    ta::SharedResources sr{};

    // GIVEN -- a landing event send to the vertiport
    ta::EventType landing{0.1, ta::Cause::land, VERTIPORT_ID, AC_1_ID};

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

TEST(Vertiport, start_charging_nominal)
{
    ta::Vertiport       uut{VERTIPORT_ID, 3U};
    ta::SharedResources sr{};

    // GIVEN -- a charging event sent to the vertiport
    ta::EventType charging{0.1, ta::Cause::start_charging, VERTIPORT_ID, AC_1_ID};

    // WHEN -- AC_1 starts charging
    uut.start_charging(charging, sr);

    // THEN -- one is charging (no new events are posted)
    EXPECT_TRUE(sr.m_queue.empty());

    EXPECT_EQ(1U, uut.charging());
    EXPECT_EQ(0U, uut.waiting());
}

TEST(Vertiport, stop_charging_nominal)
{
    ta::Vertiport       uut{VERTIPORT_ID, 3U};
    ta::SharedResources sr{};
    ta::EventType       charging{0.1, ta::Cause::start_charging, VERTIPORT_ID, AC_1_ID};
    uut.start_charging(charging, sr);

    // GIVEN -- a complete charging event sent to the vertiport
    ta::EventType complete{0.2, ta::Cause::complete_charging, VERTIPORT_ID, AC_1_ID};

    // WHEN -- AC_1 completes charging
    uut.complete_charging(complete, sr);

    // THEN -- none are charging (no new events are posted)
    EXPECT_TRUE(sr.m_queue.empty());

    EXPECT_EQ(0U, uut.charging());
    EXPECT_EQ(0U, uut.waiting());
}

TEST(Vertiport, process_landing_full)
{
    ta::Vertiport       uut{VERTIPORT_ID, 2U};
    ta::SharedResources sr{};

    // GIVEN -- two are charging (the vertiport is full)
    ta::EventType landing{0.1, ta::Cause::land, VERTIPORT_ID, AC_1_ID};
    uut.process_landing(landing, sr);
    ta::EventType event = sr.m_queue.pop();
    ta::EventType charging{0.1, ta::Cause::start_charging, VERTIPORT_ID, AC_1_ID};
    uut.start_charging(charging, sr);
    landing = {0.2, ta::Cause::land, VERTIPORT_ID, AC_2_ID};
    uut.process_landing(landing, sr);
    event    = sr.m_queue.pop();
    charging = {0.2, ta::Cause::start_charging, VERTIPORT_ID, AC_2_ID};
    uut.start_charging(charging, sr);
    EXPECT_EQ(2U, uut.charging());

    // WHEN -- AC_3 lands
    landing = {0.3, ta::Cause::land, VERTIPORT_ID, AC_3_ID};
    uut.process_landing(landing, sr);

    // THEN -- There are no free chargers so no charging event is sent to AC_3
    EXPECT_TRUE(sr.m_queue.empty());
    EXPECT_EQ(2U, uut.charging());
    EXPECT_EQ(1U, uut.waiting());

    // AND WHEN -- AC_1 completes charging
    ta::EventType complete{0.4, ta::Cause::complete_charging, VERTIPORT_ID, AC_1_ID};
    uut.complete_charging(complete, sr);

    // THEN -- a charger frees up so a start charging is sent to AC_3
    ASSERT_FALSE(sr.m_queue.empty());
    event = sr.m_queue.pop();
    EXPECT_DOUBLE_EQ(0.4, event.time());
    EXPECT_EQ(AC_3_ID, event.aircraft());
    EXPECT_EQ(ta::Cause::start_charging, event.cause());
    EXPECT_EQ(VERTIPORT_ID, event.charger());

    EXPECT_EQ(1U, uut.charging());
    EXPECT_EQ(0U, uut.waiting());
}
