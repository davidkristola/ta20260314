#pragma once

namespace ta {

using AircraftId = unsigned long int;
using ChargerId  = unsigned long int;

using HoursType                = double;
using MilesType                = double;
using MilesPerHourType         = double;
using KilowattHoursType        = double;
using KilowattHoursPerMileType = double;
using KilowattHoursPerHourType = double;
using ProbabilityPerHourType   = double;
using PassengerMilesType       = double;

using PassengerCountType     = unsigned int;
using FlightCountType        = unsigned int;
using FaultCountType         = unsigned int;
using ChargeSessionCountType = unsigned int;
using AircraftCountType      = unsigned int;
using ChargerCountType       = unsigned int;

enum class Cause { end_of_simulation, take_off, land, start_charging, complete_charging, experience_fault };

} // namespace ta
