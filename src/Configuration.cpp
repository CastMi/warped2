#include "Configuration.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <iostream>

#include "json/json.h"

#include "config.h"
#include "CommandLineConfiguration.hpp"
#include "EventDispatcher.hpp"
#include "Partitioner.hpp"
#include "STLLTSFQueue.hpp"
#include "ProfileGuidedPartitioner.hpp"
#include "RoundRobinPartitioner.hpp"
#include "SequentialEventDispatcher.hpp"
#include "utility/memory.hpp"

namespace {
const static std::string DEFAULT_CONFIG = R"x({

// If max-sim-time > 0, the simulation will halt once the time has been reached
"max-sim-time": 0,

// Valid options are "sequential" and "time-warp"
"simulation-type": "time-warp",

"statistics": {
    // Valid options are "none", "json", "csv", "graphviz", and "metis".
    // "json" and "csv" are individual statistics, others are aggregate.
    "type": "none",
    // If statistics-type is not "none", save the output in this file
    "file": "statistics.out"
},

"time-warp" : {
    "gvt-calculation": {
        "period": 1000
    },

    "state-saving": {
        "type": "periodic",
        "period": 10
    },

    "cancellation": "aggressive",

    "worker-threads": 3,

    "scheduler-count": 1,

    // LP Migration valid options are "on" and "off"
    "lp-migration": "off",

    // Name of file to dump stats, "none" to disable
    "statistics-file" : "none",

    "config-output-file" : "none",

    "communication" : {
        "max-buffer-size" : 512
    }
},

"partitioning": {
    // Valid options are "default", "round-robin" and "profile-guided".
    // "default" will use user provided partitioning if given, else
    // "round-robin".
    "type": "default",
    // The path to the statistics file that was created from a previous run.
    // Only used if "partitioning-type" is "profile-guided".
    "file": "statistics.out"
}

})x";


// Recursively copy the values of b into a. Both a and b must be objects.
void update(Json::Value& a, Json::Value& b) {
    if (!a.isObject() || !b.isObject()) { return; }

    for (const auto& key : b.getMemberNames()) {
        if (a[key].isObject()) {
            update(a[key], b[key]);
        } else {
            a[key] = b[key];
        }
    }
}

Json::Value parseJsonFile(std::string filename) {
    Json::Reader reader;
    std::ifstream input(filename);

    if (!input.is_open()) {
        throw std::runtime_error(std::string("Could not open configuration file ") + filename);
    }

    Json::Value root;
    auto success = reader.parse(input, root);
    input.close();

    if (!success) {
        throw std::runtime_error(std::string("Failed to parse configuration\n") +
                                 reader.getFormattedErrorMessages());
    }

    return root;
}

} // namespace

namespace warped {

Configuration::Configuration(const std::string& config_file_name)
    : config_file_name_(config_file_name), root_(nullptr)
{ readUserConfig(); }

Configuration::Configuration(const std::string& model_description, int argc,
                             const char* const* argv,
                             const std::vector<TCLAP::Arg*>& cmd_line_args)
    : config_file_name_(""), root_(make_unique<Json::Value>())
{ init(model_description, argc, argv, cmd_line_args); }

Configuration::Configuration(const std::string& model_description, int argc,
                             const char* const* argv)
    : config_file_name_(""), root_(make_unique<Json::Value>())
{ init(model_description, argc, argv, {}); }

Configuration::~Configuration() = default;

void Configuration::readUserConfig() {
    if (!config_file_name_.empty()) {
        auto root2 = parseJsonFile(config_file_name_);
        update(*root_, root2);
    }
}

void Configuration::init(const std::string& model_description, int argc, const char* const* argv,
                         const std::vector<TCLAP::Arg*>& cmd_line_args) {
    Json::Reader reader;

    if (!reader.parse(DEFAULT_CONFIG, *root_)) {
        throw std::runtime_error(std::string("Failed to parse default configuration\n") +
                                 reader.getFormattedErrorMessages());
    }

    // Update the JSON root with any command line args given
    bool should_dump;
    CommandLineConfiguration clc(*root_);
    std::tie(should_dump, config_file_name_) = clc.parse(model_description, argc, argv, cmd_line_args);

    readUserConfig();

    if (should_dump) {
        std::cout << *root_ << std::endl;
        std::exit(0);
    }
}

std::unique_ptr<EventDispatcher>
Configuration::makeDispatcher() {
    std::string invalid_string;

    auto simulation_type = (*root_)["simulation-type"].asString();

    std::cout << "Simulation type: Sequential" << std::endl;

    return make_unique<SequentialEventDispatcher>();
}

std::unique_ptr<Partitioner> Configuration::makePartitioner() {
   return make_unique<RoundRobinPartitioner>();
}

std::unique_ptr<Partitioner>
Configuration::makePartitioner(std::unique_ptr<Partitioner> ) {
    return makePartitioner();
}

std::unique_ptr<Partitioner> Configuration::makeLocalPartitioner(unsigned int ,
    unsigned int& ) {
   return makePartitioner();
}

} // namespace warped

