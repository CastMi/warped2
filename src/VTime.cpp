
#include <stdlib.h>                     // for abort

#include "VTime.hpp"

VTime::~VTime() {}

bool VTime::operator< (const VTime& right) const {
   return (timestamp_ < right.timestamp_) ? true :
      ((timestamp_ != right.timestamp_) ? false :
       ((regions_ < right.regions_) ? true :
        ((regions_ != right.regions_) ? false :
         ((delta_cycle_ < right.delta_cycle_) ? true :
          ((delta_cycle_ != right.delta_cycle_) ? false :
           ((region_state_ < right.region_state_) ? true : false))))));
}

bool VTime::operator> (const VTime& right) const {
   return !(*this <= right);
}

bool VTime::operator== (const VTime& right) const {
   return timestamp_ == right.timestamp_ &&
      regions_ == right.timestamp_ &&
      delta_cycle_ == right.delta_cycle_ &&
      region_state_ == right.region_state_;
}

bool VTime::operator!= (const VTime& right) const {
   return !(*this == right);
}

bool VTime::operator<= (const VTime& right) const {
   return *this < right ||
      *this == right;
}

bool VTime::operator>= (const VTime& right) const {
   return !(*this < right);
}

const VTime VTime::operator+ (const VTime& right) const {
   VTime tmp(*this);
   tmp.timestamp_ += right.timestamp_;
   return tmp;
}

const VTime VTime::operator+ (unsigned int right) const { 
   VTime tmp(*this);
   tmp.timestamp_ += right;
   return tmp;
}

VTime& VTime::operator+= (unsigned int right) {
   timestamp_ += right;
   return *this;
}

VTime& VTime::operator+= (const VTime& right) {
   timestamp_ += right.timestamp_;
   return *this;
}

const VTime VTime::operator++ () const { 
   VTime tmp(*this);
   tmp.timestamp_ += 1;
   return tmp;
}

