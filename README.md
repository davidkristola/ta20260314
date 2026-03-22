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
$ cmake .. && cmake --build . && ./ta_tests

Note that the unit tests require Google Test.

# Assumptions
1 (given) Each vehicle starts the simulation with a fully-charged battery
2 (given) Each vehicle instantaneously reaches Cruise Speed
3 (given) Each vehicle is airborne for the full use of the battery
4 (given) Each vehicle is immediately in line for the charger after running out of battery power
5 (assumed) Each vehicle takes off once fully charged
6 (assumed) Each vehicle carries the maximum number of passenger each trip
7 (assumed) The random selection of vehicles for each simulation must guarantee at least one of each type
8 (assumed) Upon failure, the vehicle immediately lands, does not recharge, and is out of the simulation
