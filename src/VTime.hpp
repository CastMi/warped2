#ifndef VTIME_H
#define VTIME_H


#include <stdlib.h>
#include <iostream>                     // for ostream
#include <sstream>
#include <climits>
#include <string>                       // for operator<<, string

enum RegionState {
   Preponed,
   Active,
   Inactive,
   NBA,
   Observed,
   Reactive,
   Re_Inactive,
   Re_NBA,
   Postponed
};

class VTime {
public:
    VTime(unsigned int timestamp, unsigned int delta_cycle, unsigned int regions,
          RegionState region_state = Preponed) : timestamp_(timestamp), delta_cycle_(delta_cycle),
          regions_(regions), region_state_(region_state) {};
    virtual ~VTime();

    virtual bool operator< (const VTime& right) const;
    virtual bool operator> (const VTime& right) const;
    virtual bool operator== (const VTime& right) const;
    virtual bool operator!= (const VTime& right) const;
    virtual bool operator<= (const VTime& right) const;
    virtual bool operator>= (const VTime& right) const;

    virtual const VTime operator+ (const VTime& right) const; 
    virtual const VTime operator+ (unsigned int right) const;
    virtual VTime& operator+= (const VTime& right);
    virtual VTime& operator+= (unsigned int right);
    virtual const VTime operator++ () const; 

    static const VTime& getPositiveInfinity() { 
       static const VTime infinite = VTime(UINT_MAX, UINT_MAX, UINT_MAX, RegionState::Postponed);
       return infinite;
    };
    static  VTime& getZero() {
       static VTime Zero = VTime(0, 0, 0, RegionState::Preponed);
       return Zero;
    };

    virtual const std::string toString() const { return std::to_string(timestamp_); };
    static const VTime& getMaxSimTime() { return getPositiveInfinity(); };
    unsigned int getApproximateIntTime() const { return timestamp_; }

    unsigned int timestamp_, delta_cycle_, regions_;
    RegionState region_state_;

};

inline
std::ostream& operator<< (std::ostream& os, const VTime& time) {
    os << time.toString();
    return os;
}

#endif
