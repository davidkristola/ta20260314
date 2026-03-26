#pragma once
/// @file

#include "configuration.hpp"
#include "sequencer.hpp"
#include "statistics.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace ta {

/// @brief A class to encapsulate the simulation.
//
class Simulation {
    Configuration m_configuration{};

public:
    /// @brief Set up the simulation using the comman line arguments, and run until done.
    /// @param args Command line arguments
    //
    void setup_and_run(const std::vector<std::string>& args);

private:
    /// @brief Report statistics at the end of the run.
    /// @param simulation
    //
    void report(Sequencer& simulation);

    /// @brief Print out a particular aircraft type's statistics.
    /// @param vehicle_type_name
    /// @param statistics
    //
    void show_statistics_for(const std::string_view& vehicle_type_name, const Statistics& statistics);
};

} // namespace ta
