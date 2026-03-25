#pragma once
/// @file

#include "common_types.hpp"

#include <random>

namespace ta {

/// Model aircraft faults.
/// The FaultModel class provides two different types of randomness for the simulation,
/// the aircraft flight failures, and a random index for selecting types of aircraft
/// to deploy in the simulation. This second feature is not a fault model. It is combined
/// here so that the simulation can have a single random number generator.
///
/// The point of using random numbers in a simulation is to enable Monte Carlo simulations.
/// This implies that the simulation will be run many times, each with a different seed
/// for the generator. Additionally, there should be a capability to re-run a simulation
/// using the same seed (and getting the same results).
//
// TODO: find a better name for this class.
//
class FaultModel {
    std::mt19937 m_generator;

public:
    FaultModel() noexcept;
    void          seed(unsigned long seed) noexcept;
    HoursType     time_to_next_fault(ProbabilityPerHourType probability) noexcept;
    unsigned long random_index(unsigned long upper_bound) noexcept;
};

} // namespace ta
