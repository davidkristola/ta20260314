#include "event_queue.hpp"

#include "gtest/gtest.h"

TEST(EventQueue, standard_use_case) {
    ta::EventQueue uut;
    EXPECT_TRUE(uut.empty());
    ta::EventType e1{1, ta::Cause::end_of_simulation, 0.3};
    uut.push(e1);
    EXPECT_FALSE(uut.empty());
    uut.push(ta::EventType{2, ta::Cause::land, 0.2});
    uut.push(ta::EventType{3, ta::Cause::take_off, 0.1});
    EXPECT_DOUBLE_EQ(0.1, uut.top_time());

    e1 = uut.pop();
    EXPECT_EQ(3, e1.id());
    EXPECT_DOUBLE_EQ(0.2, uut.top_time());

    e1 = uut.pop();
    EXPECT_EQ(2, e1.id());
    EXPECT_DOUBLE_EQ(0.3, uut.top_time());

    e1 = uut.pop();
    EXPECT_EQ(1, e1.id());
    EXPECT_TRUE(uut.empty());
}
