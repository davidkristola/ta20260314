#pragma once
/// @file
/// Common and simple (POD) types used widely across the codebase.

/// The technical assessment namespace.
///
/// All could written for this exercise should be in a unique namespace to avoid
/// name collisions with third party libraries.
namespace ta {

/// A unique identifier for each simulated object.
using SimEntityId = unsigned long int;

/// Invalid simulated object identifier.
constexpr SimEntityId InvalidSimEntityId = 0U;

///@{
/// Unit type.
using HoursType                = double;
using MilesType                = double;
using MilesPerHourType         = double;
using KilowattHoursType        = double;
using KilowattHoursPerMileType = double;
using KilowattHoursPerHourType = double;
using ProbabilityPerHourType   = double;
using PassengerMilesType       = double;
///@}

/// This type is used to count passengers.
using PassengerCountType = unsigned long int;

/// This type is used to count flights.
using FlightCountType = unsigned long int;

/// This type is used to count how many times an aircraft has a fault.
using FaultCountType = unsigned long int;

/// This type is used to count how many times an aircraft charges.
using ChargeSessionCountType = unsigned long int;

/// This type is used to count aircraft.
using AircraftCountType = unsigned long int;

/// This type is used to count chargers.
using ChargerCountType = unsigned long int;

/// This type is used to seed a random number generator.
using SeedType = unsigned int;

/// The cause (reason, action) for a simulation event.
enum class Cause { end_of_simulation, take_off, land, start_charging, complete_charging, experience_fault };

} // namespace ta
