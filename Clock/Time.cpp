#include <Arduino.h>

#include "Time.h"

Time::Time(void){
#ifndef _DEBUG_MOCK_RTC
  this->rtc.begin();
  this->update_time(true);
#else
  this->datetime[YEAR] = 20;
  this->datetime[MONTH] = 8;
  this->datetime[DAY] = 12;
  this->datetime[HOUR] = 5;
  this->datetime[MINUTE] = 5;
  this->datetime[SECOND] = 45;
#endif
}

// Get the relevant information from the RTC
byte Time::get_rtc(int section) {
  switch (section) {
#ifdef _DEBUG_MOCK_RTC
    case YEAR:   return (this->datetime[section] + 1) % 99;
    case MONTH:  return (this->datetime[section] + 1) % 13;
    case DAY:    return (this->datetime[section] + 1) % 32;
    case HOUR:   return (this->datetime[section] + 1) % 24;
    case MINUTE: return (this->datetime[section] + 1) % 60;
    case SECOND: return (this->datetime[section] + 1) % 60;
#else
    case YEAR:   return rtc.getYear();
    case MONTH:  return rtc.getMonth();
    case DAY:    return rtc.getDay();
    case HOUR:   return rtc.getHours();
    case MINUTE: return rtc.getMinutes();
    case SECOND: return rtc.getSeconds();
    default:     return 99;
#endif    
  }
}

// Update all date and time values
void Time::update_time(bool force, int section) {

  // Stop if we are past the Second position
  if (section < 0 || section >= SECTIONS) { return; }

  this->datetime[section] = this->get_rtc(section);

  if (force || this->datetime[section] == 0) {
    this->update_time(force, section - 1);
  }
}
