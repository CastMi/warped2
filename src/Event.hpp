#ifndef WARPED_EVENT_HPP
#define WARPED_EVENT_HPP

#include <string>
#include "serialization.hpp"
#include "VTime.hpp"

namespace warped {

struct compareEvents;

enum class EventType : bool {
    NEGATIVE = 0,
    POSITIVE
};

// Events are passed between objects. They may contain data, and must be
// serializable. See serialization.hpp for info on serializing Events.
class Event {
public:
    Event(EventType type = EventType::POSITIVE) : event_type_(type), generation_(0) {};
    virtual ~Event() {}

    bool operator== (const Event &other) {
        return ((this->timestamp() == other.timestamp())
                && (this->send_time_ == other.send_time_)
                && (this->sender_name_ == other.sender_name_)
                && (this->generation_ == other.generation_));
    }

    bool operator< (const Event &other) {
        return  (this->timestamp() < other.timestamp()) ? true :
                ((this->timestamp() != other.timestamp()) ? false :
                  ((this->send_time_ < other.send_time_) ? true :
                  ((this->send_time_ != other.send_time_) ? false :
                    ((this->sender_name_ < other.sender_name_) ? true :
                    ((this->sender_name_ != other.sender_name_) ? false :
                      ((this->generation_ < other.generation_) ? true :
                      ((this->generation_ != other.generation_) ? false : false)))))));
    }

    bool operator<= (const Event &other) {
        return (*this < other) || (*this == other);
    }

    bool operator>= (const Event &other) {
        return !(*this < other);
    }

    bool operator> (const Event &other) {
        return !(*this <= other);
    }

    // The name of the SimualtionObject that should receive this event.
    virtual const std::string& receiverName() const = 0;

    // The timestamp of when the event should be received.
    virtual const VTime& timestamp() const = 0;

    // The name of the SimualtionObject that sends this event.
    std::string sender_name_;

    // Event type - positive or negative
    EventType event_type_;

    // Send time
    const VTime* send_time_;

    // For differentiating same events which is caused by
    //  anti-message + regeneration of event.
    unsigned long long generation_;

    WARPED_REGISTER_SERIALIZABLE_MEMBERS(sender_name_, event_type_, generation_)

};

class NegativeEvent : public Event {
public:
    NegativeEvent() : receive_time_(&VTime::getZero()) {};
    NegativeEvent(std::shared_ptr<Event> e) {
        receive_time_ = &(e->timestamp());
        receiver_name_ = e->receiverName();
        event_type_ = EventType::NEGATIVE;
        generation_ = e->generation_;
    }

    const std::string& receiverName() const { return receiver_name_; }
    const VTime& timestamp() const { return *receive_time_; }

    std::string receiver_name_;
    const VTime* receive_time_;

    WARPED_REGISTER_SERIALIZABLE_MEMBERS(cereal::base_class<Event>(this), receiver_name_)
};

// Initial event used with the initial state save of all objects
class InitialEvent : public Event {
public:
    InitialEvent() {
   }

    const std::string& receiverName() const { return receiver_name_; }
    const VTime& timestamp() const { return VTime::getZero(); }

    std::string receiver_name_ = "";
};

/* Compares two events to see if one has a receive time less than to the other */
struct compareEvents {
public:
    bool operator() (const std::shared_ptr<Event>& first,
                     const std::shared_ptr<Event>& second) const {
        return  (first->timestamp() < second->timestamp()) ? true :
                ((first->timestamp() != second->timestamp()) ? false :
                  ((first->send_time_ < second->send_time_) ? true :
                  ((first->send_time_ != second->send_time_) ? false :
                    ((first->sender_name_ < second->sender_name_) ? true :
                    ((first->sender_name_ != second->sender_name_) ? false :
                      ((first->generation_ < second->generation_) ? true :
                      ((first->generation_ != second->generation_) ? false :
                        ((first->event_type_ < second->event_type_) ? true :
                        ((first->event_type_ != second->event_type_) ? false : false)))))))));
    }
};

} // namespace warped

#endif
