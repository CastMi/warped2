#include "Simulation.hpp"

#include <memory>
#include <vector>
#include <tuple>
#include <cassert>
#include <algorithm>

#include "Configuration.hpp"
#include "EventDispatcher.hpp"
#include "Partitioner.hpp"
#include "LogicalProcess.hpp"

extern "C" {
    void warped_is_present(void) {
        return;
    }
}

namespace warped {

std::unique_ptr<EventDispatcher> Simulation::event_dispatcher_;

Simulation::Simulation(const std::string& model_description, int argc, const char* const* argv,
                       const std::vector<TCLAP::Arg*>& cmd_line_args)
    : config_(model_description, argc, argv, cmd_line_args) {}

Simulation::Simulation(const std::string& model_description, int argc, const char* const* argv)
    : config_(model_description, argc, argv) {}

Simulation::Simulation(const std::string& config_file_name)
    : config_(config_file_name) {}

void Simulation::simulate(const std::vector<LogicalProcess*>& lps) {
    check(lps);

    event_dispatcher_ = config_.makeDispatcher();
    event_dispatcher_->startSimulation(lps);
}

void Simulation::check(const std::vector<LogicalProcess*>& lps) {
   if(std::adjacent_find(lps.begin(), lps.end(), equalNames) != lps.end())
      throw std::runtime_error(std::string("Two LogicalProcess with the same name."));
}

FileStream& Simulation::getFileStream(LogicalProcess* lp, const std::string& filename,
    std::ios_base::openmode mode, std::shared_ptr<Event> this_event) {

    return event_dispatcher_->getFileStream(lp, filename, mode, this_event);
}

} // namepsace warped
