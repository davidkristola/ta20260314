#include "statistics.hpp"

namespace ta {

Statistics::Statistics(const AircraftType& aircraft_type) noexcept : m_name(aircraft_type.m_name) {}

FlightCountType Statistics::total_flights() const noexcept { return m_flight_count; }

ChargeSessionCountType Statistics::total_charge_sessions() const noexcept { return m_charge_sessions; }

PassengerMilesType Statistics::total_passenger_miles() const noexcept { return m_accumulated_passenger_miles; }

FaultCountType Statistics::total_faults() const noexcept { return m_faults; }

void Statistics::record_fault() noexcept { m_faults += 1U; }

void Statistics::record_charging_session(HoursType hours) noexcept {
    m_charge_sessions += 1U;
    m_accumulated_charge_time += hours;
}

void Statistics::record_flight(HoursType hours, MilesType miles, PassengerCountType passengers) noexcept {
    m_flight_count += 1U;
    m_accumulated_flight_hours += hours;
    m_accumulated_flight_miles += miles;
    m_accumulated_passenger_miles += (miles * static_cast<PassengerMilesType>(passengers));
}

HoursType Statistics::average_hours_per_flight() const noexcept {
    if (m_flight_count == 0U) {
        return 0.0;
    }
    const auto count = static_cast<HoursType>(m_flight_count);
    return m_accumulated_flight_hours / count;
}

MilesType Statistics::average_miles_per_flight() const noexcept {
    if (m_flight_count == 0U) {
        return 0.0;
    }
    const auto count = static_cast<MilesType>(m_flight_count);
    return m_accumulated_flight_miles / count;
}

HoursType Statistics::average_hours_per_charge() const noexcept {
    if (m_charge_sessions == 0U) {
        return 0.0;
    }
    const auto count = static_cast<HoursType>(m_charge_sessions);
    return m_accumulated_charge_time / count;
}

} // namespace ta
