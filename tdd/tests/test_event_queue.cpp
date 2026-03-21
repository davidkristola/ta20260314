#include "event_queue.hpp"

#include "gtest/gtest.h"

TEST(EventQueue, standard_use_case)
{
    ta::EventQueue uut;
    EXPECT_TRUE(uut.empty());
    ta::EventType test_event{0.3, ta::Cause::end_of_simulation};
    uut.push(test_event);
    EXPECT_FALSE(uut.empty());
    uut.push(ta::EventType{0.2, ta::Cause::land, 8U});
    uut.push(ta::EventType{0.1, ta::Cause::take_off, 8U});
    EXPECT_DOUBLE_EQ(0.1, uut.top_time());

    test_event = uut.pop();
    EXPECT_DOUBLE_EQ(0.1, test_event.time());
    EXPECT_DOUBLE_EQ(0.2, uut.top_time());

    test_event = uut.pop();
    EXPECT_DOUBLE_EQ(0.2, test_event.time());
    EXPECT_DOUBLE_EQ(0.3, uut.top_time());

    test_event = uut.pop();
    EXPECT_DOUBLE_EQ(0.3, test_event.time());
    EXPECT_TRUE(uut.empty());
}
