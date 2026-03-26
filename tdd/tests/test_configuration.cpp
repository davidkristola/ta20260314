#include "configuration.hpp"

#include "gtest/gtest.h"

TEST(Configuration, no_seed)
{
    ta::Configuration uut;
    const auto        old_seed = uut.seed;
    uut.process_command_line_arguments({"executable", "arg1"});
    EXPECT_EQ(old_seed, uut.seed);
}

TEST(Configuration, good_seed)
{
    ta::Configuration uut;
    const auto        old_seed = uut.seed;
    uut.process_command_line_arguments({"executable", "--seed", "294243363"});
    EXPECT_NE(old_seed, uut.seed);
    EXPECT_EQ(294243363, uut.seed);
}

TEST(Configuration, bad_seed)
{
    ta::Configuration uut;
    const auto        old_seed = uut.seed;
    uut.process_command_line_arguments({"executable", "--seed", "foobar"});
    EXPECT_EQ(old_seed, uut.seed);
}

TEST(Configuration, run_time)
{
    ta::Configuration uut;
    uut.process_command_line_arguments({"executable", "--rt", "20"});
    EXPECT_EQ(20.0, uut.run_time);
}

TEST(Configuration, aircraft_count)
{
    ta::Configuration uut;
    uut.process_command_line_arguments({"executable", "--ac", "50"});
    EXPECT_EQ(50, uut.aircraft_count);
}

TEST(Configuration, charger_count)
{
    ta::Configuration uut;
    uut.process_command_line_arguments({"executable", "--cc", "10"});
    EXPECT_EQ(10, uut.charger_count);
}

TEST(Configuration, disable_faults)
{
    ta::Configuration uut;
    uut.process_command_line_arguments({"executable", "--df"});
    EXPECT_TRUE(uut.disable_faults);
}
