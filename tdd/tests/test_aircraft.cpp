#include "aircraft.hpp"

#include "gtest/gtest.h"

TEST(Aircraft, init) {
    ta::AircraftType typ{"Bumble Bee", 100.0, 500.0, 0.5, 1.0, 2U, 0.13};
    ta::Aircraft uut{typ, 32U};
    EXPECT_EQ(uut.current_charge(), 500.0);
}
