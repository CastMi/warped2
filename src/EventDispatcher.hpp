#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

#include <limits>
#include <vector>

#include "FileStream.hpp"
#include "VTime.hpp"

namespace warped {

class LogicalProcess;
class Event;

// An EventDispatcher is responsible for sending events between lps.
class EventDispatcher {
public:
    // if max_sim_time is greater than 0, events will be processed until the
    // timestamp of all remaining events is greater than max_sim_time.
    // Otherwise, events will be processed until none remain.
    EventDispatcher()
        : max_sim_time_(VTime::getMaxSimTime()) {}
    virtual ~EventDispatcher() {}

    // This method will call createInitialEvents() on all lps, then
    // process events until a termination condition is reached.
    // The lps argument is a partitioned set of LogicalProcess
    virtual void startSimulation(const std::vector<LogicalProcess*>& lps) = 0;

    virtual FileStream& getFileStream(LogicalProcess* lp, const std::string& filename,
        std::ios_base::openmode mode, std::shared_ptr<Event> this_event) = 0;

protected:
    const VTime& max_sim_time_;
};

} // namespace warped

#endif
