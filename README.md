# ta20260314

Tools: gcc, cmake

To build the technical assessment:
$ cd src
$ mkdir build
$ cd build
$ cmake .. && cmake --build . && ./technical_assessment

This will write the telemetry to "telemetry.txt" in the directory you run from.

To build and run the unit tests:
$ cd tdd
$ mkdir build
$ cd build
$ cmake .. && cmake --build . && ./technical_assessment_tests

Note that the unit tests require Google Test.
