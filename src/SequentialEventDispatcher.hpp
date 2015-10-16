#ifndef SEQUENTIAL_EVENT_DISPATCHER_HPP
#define SEQUENTIAL_EVENT_DISPATCHER_HPP

#include <memory>
#include <unordered_map>

#include "EventDispatcher.hpp"

namespace warped {

class LogicalProcess;

// This class is an EventDispatcher that runs on a single thread and process.
class SequentialEventDispatcher : public EventDispatcher {
public:
    SequentialEventDispatcher();

    void startSimulation(const std::vector<LogicalProcess*>& lps);

    FileStream& getFileStream(LogicalProcess* lp, const std::string& filename,
        std::ios_base::openmode mode, std::shared_ptr<Event> this_event);

private:
    std::unordered_map<std::string, std::unique_ptr<FileStream, FileStreamDeleter>>
        file_stream_by_filename_;

    std::unordered_map<std::string, LogicalProcess*> lp_by_filename_;

};

} // namespace warped

#endif
