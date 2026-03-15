#include "gtest/gtest.h"

int main(int argc, char **argv) {
    // I have found it useful to check for "-p" and pause the testing
    // so that I can attach a debugger.
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
