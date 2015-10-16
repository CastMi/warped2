#include "SequentialEventDispatcher.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Event.hpp"
#include "EventDispatcher.hpp"
#include "LogicalProcess.hpp"
#include "STLLTSFQueue.hpp"
#include "utility/memory.hpp"
#include "utility/warnings.hpp"

namespace warped {

SequentialEventDispatcher::SequentialEventDispatcher()
    : EventDispatcher() {}

void SequentialEventDispatcher::startSimulation(
    const std::vector<LogicalProcess*>& lps) {
    if (lps.size() <= 1) {
        throw std::runtime_error(std::string("Simulation should contain at least 2 elements."));
    }

    std::unordered_map<std::string, LogicalProcess*> lps_by_name;
    STLLTSFQueue events;

    for (auto& lp : lps) {
        auto new_events = lp->initializeLP();
        for (auto& e : new_events) {
            if (e->timestamp() > max_sim_time_) continue;
            e->send_time_ = &VTime::getZero();
            e->sender_name_ = lp->name_;
            e->generation_ = lp->generation_++;
            events.push(e);
        }
        lps_by_name[lp->name_] = lp;
    }

    int count = 0;
    while (!events.empty()) {
        auto event = events.pop();
        auto receiver = lps_by_name[event->receiverName()];
        auto new_events = receiver->receiveEvent(*event.get());
        for (auto& e : new_events) {
            if (e->timestamp() > max_sim_time_) continue;
            e->sender_name_ = receiver->name_;
            e->send_time_   = &event->timestamp();
            e->generation_ = receiver->generation_++;
            events.push(e);
        }
        count++;
    }

    std::cout << "Events processed: " << count << std::endl;
}

FileStream& SequentialEventDispatcher::getFileStream(LogicalProcess* lp,
    const std::string& filename, std::ios_base::openmode mode, std::shared_ptr<Event> this_event) {

    unused(this_event);

    if (lp_by_filename_.count(filename) == 0) {
        // Create filestream and insert into filename to stream map
        std::unique_ptr<FileStream, FileStreamDeleter> fs(new FileStream(filename, mode),
            FileStreamDeleter());
        file_stream_by_filename_[filename] = std::move(fs);

        // Insert into filename to lp map
        lp_by_filename_[filename] = lp;

        return *(file_stream_by_filename_[filename].get());

    } else if (lp != lp_by_filename_[filename]) {
        throw std::runtime_error(std::string("A filestream can only be used by a single lp"));
    } else {
        return *(file_stream_by_filename_[filename].get());
    }
}

} // namespace warped
