#pragma once

namespace ta {

using AircraftId = unsigned long int;
using ChargerId  = unsigned long int;

using HoursType                   = double;
using MilesType                   = double;
using MilesPerHourType            = double;
using KilowattsPerHourType        = double;
using KilowattsPerHourPerMileType = double;
using PassengerCountType          = unsigned int;
using ProbabilityType             = double;

enum class Cause { end_of_simulation, take_off, land, start_charging, complete_charging, experience_fault };

} // namespace ta
