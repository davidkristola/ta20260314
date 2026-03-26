# ta20260314
# Overview
This is a technical assessment (take home assignment) for a company that shall remain nameless.

The goal is to write an eVTOL Simulation (in C++), and show design, code, documentation, and testing.

# Build

Tools: gcc, cmake, doxygen (plus dot and PlantUML)

To build the technical assessment:
```bash
cd src
mkdir build
cd build
cmake .. && cmake --build . && ./technical_assessment
```

## Build some of the UML diagrams
```bash
java -jar "/path/to/plantuml.jar" doc_inc/*.puml
```

# Unit tests

Note that the unit tests require Google Test.

To build and run the unit tests:
```bash
cd tdd
mkdir build
cd build
cmake .. && cmake --build . && ./ta_tests
```

# Assumptions
1. (given) Each vehicle starts the simulation with a fully-charged battery
2. (given) Each vehicle instantaneously reaches Cruise Speed
3. (given) Each vehicle is airborne for the full use of the battery
4. (given) Each vehicle is immediately in line for the charger after running out of battery power
5. (assumed) Each vehicle takes off once fully charged
6. (assumed) Each vehicle carries the maximum number of passenger each trip
7. (assumed) The random selection of vehicles for each simulation must guarantee at least one of each type
8. (assumed) Upon failure, the vehicle immediately lands, does not recharge, and is out of the simulation

# Running

The executable is not deployed by the CMake build script. The executable must run from the `src/build` directory:
```bash
src/build/technical_assessment
```

No command line arguments are needed, but a number have been provided. Use `--help` to see the list. Without command line arguments,
the simulation will randomly select 20 aircraft (picked from the built in 5 types) (see assumption #7), create 3 chargers, and run
for 3 simulated hours.

```text
command line options:
 --help    print this info.
 --seed #  set the sim run seed to # (unsigned int in decimal).
 --rt #    set the sim run time to # (double hours).
 --ac #    set the sim aircraft count to # (unsigned int in decimal).
 --cc #    set the sim charger count to # (unsigned int in decimal).
 --df      disable faults.
```

# Output

Typical output will look like this:
```text
$ src/build/technical_assessment

running with seed 3038430942


******** Statistics ********
Statistics for Alpha
    average flight time per flight          : 1.66667
    average distance traveled per flight    : 200
    average time charging per charge session: 0.6
    total number of faults                  : 3
    total number of passenger miles         : 1600
    total vehicles in sim                   : 4
Statistics for Bravo
    average flight time per flight          : 0.666667
    average distance traveled per flight    : 66.6667
    average time charging per charge session: 0.2
    total number of faults                  : 0
    total number of passenger miles         : 6000
    total vehicles in sim                   : 6
Statistics for Charlie
    average flight time per flight          : 0.625
    average distance traveled per flight    : 100
    average time charging per charge session: 0.8
    total number of faults                  : 0
    total number of passenger miles         : 1200
    total vehicles in sim                   : 2
Statistics for Delta
    average flight time per flight          : 1.66667
    average distance traveled per flight    : 150
    average time charging per charge session: 0.62
    total number of faults                  : 2
    total number of passenger miles         : 600
    total vehicles in sim                   : 3
Statistics for Echo
    average flight time per flight          : 0.862069
    average distance traveled per flight    : 25.8621
    average time charging per charge session: 0.3
    total number of faults                  : 5
    total number of passenger miles         : 155.172
    total vehicles in sim                   : 5
```

As you can see in this run, 3 out of 4 Alpha vehicles experienced faults, and 5 out of 5 Echo vehicles also experienced faults.

# Design
Exceptions are thrown for precondition violation, but they are not caught. At present, the simulation does not crash. A precondition exception
indicates a coding error.

## The Simulation
The simulation is centered around a time-ordered event queue. At the start of the simulation, all aircraft take off (see assumption #5 in coordination with #1).

![Simulation](https://github.com/davidkristola/ta20260314/blob/main/doc_inc/simulation_classes.png)

## Simulation Class
This is just a holder to bind together the `Configuration` and the `Sequencer`.

## Configuration Class
This provides the parameters that shape the simulation. Default parameters are provided, but can be modified using
the command line.

## Sequencer Class
This class creates and uses the SharedResources for the simulation. Simulation steps are executed one at a time until the `end_of_simulation` event is popped from
the event queue.

The `Sequencer` creates the pool of aircraft (by picking randomly from the available types) and a single `Vertiport`. These are managed separately now, but should be
combined into a single pool of simulation entities. Also the work of creating this pool should be moved into a new class.

## SharedResources ~~Class~~ Struct
This is really just a data holder for the resources needed by a `SimEntity` to process an `EventType`.

## StatisticsPool Class
A wrapping around a `std::map` to hold `Statistics` classes.

## Statistics Class
Sets of information accumulated over a simulation run for a specific aircraft.
- average flight time per flight
- average distance traveled per flight
- average time charging per charge session
- total number of faults
- total number of passenger miles

## EventQueue Class
A wrapper around a `std::priority_queue` with added logic to enforce preconditions.

## FaultModel Class
This is an attempt to model faults that occur during aircraft flight. The model inverts the probability per flight hour to create a pseudo mean time between failures, then
uses the random number generator to pick a `std::uniform_real_distribution` value in the range. This is a naive model, but satisfies the current requirements.

## SimEntity
Base class for all simulation entities.
![SimEntity class hierarchy](https://github.com/davidkristola/ta20260314/blob/main/doc_inc/sim_entity_hierarchy.png)

### Aircraft
Class for simulated eVTOL aircraft. The code is designed to work as functions of time. Given a flight time, the batteries will discharge appropriately.
Given a charge time, the batteries will be recharged appropriately.

![Aircraft State Machine](https://github.com/davidkristola/ta20260314/blob/main/doc_inc/aircraft_state_machine.png)

### Vertiport
The `Vertiport` manages a configurable number of chargers. Once all chargers are in use, aircraft landing are placed in
a first-in-first-out queue.

## common_types.hpp
This is a collection of primitive data types that are used across the codebase. A `using` clause forms a `typedef` so the unit types are not
unique. They could be mixed inappropriately. A units library should be employed to ensure proper usage of these types.

# Future Modifications

This is an unordered list of "nice to have" features:
* Logging
* Some sort of dispatcher model so that all aircraft don't fly until their batteries are depleted
* Multiple vertiport models to dispatch aircraft to and from
* Better statistics about charger wait time
* Different types of chargers (faster, slower)
* Better statistics about factors that impact costs and income
* Use a library for units (Boost)
* Improve fault modeling
* Make `AircraftType` immutable
* Parse a configuration file for aircraft types, etc.
* Combine `Aircraft`, `Vertiport`, and other simulation entities into a unified pool
