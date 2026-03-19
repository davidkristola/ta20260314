#pragma once

#include "aircraft_type.hpp"

namespace ta {

class Statistics {
    std::string_view m_name = "";

    FlightCountType m_flight_count           = 0U;
    ChargeSessionCountType m_charge_sessions = 0U;
    FaultCountType m_faults                  = 0U;

    HoursType m_accumulated_charge_time              = 0.0;
    HoursType m_accumulated_flight_hours             = 0.0;
    MilesType m_accumulated_flight_miles             = 0.0;
    PassengerMilesType m_accumulated_passenger_miles = 0.0;

public:
    Statistics(const AircraftType& aircraft_type) noexcept;

    std::string_view name() const noexcept { return m_name; }
    FlightCountType total_flights() const noexcept;
    ChargeSessionCountType total_charge_sessions() const noexcept;
    PassengerMilesType total_passenger_miles() const noexcept;
    FaultCountType total_faults() const noexcept;

    void record_fault() noexcept;
    void record_charging_session(HoursType hours) noexcept;
    void record_flight(HoursType hours, MilesType miles, PassengerCountType passengers) noexcept;

    HoursType average_hours_per_flight() const noexcept;
    MilesType average_miles_per_flight() const noexcept;
    HoursType average_hours_per_charge() const noexcept;
};

} // namespace ta
