#include <Arduino.h>

#include "Time.h"

Time::Time(void){
#ifndef _DEBUG_MOCK_RTC
  this->rtc.begin();
  this->update_time(true);
#else
  this->datetime[DT_YEAR]   = 20;
  this->datetime[DT_MONTH]  = 8;
  this->datetime[DT_DAY]    = 12;
  this->datetime[DT_HOUR]   = 5;
  this->datetime[DT_MINUTE] = 5;
  this->datetime[DT_SECOND] = 45;
#endif
}

// Get the relevant information from the RTC
byte Time::get_rtc(int section) {
  switch (section) {
#ifdef _DEBUG_MOCK_RTC
    case DT_YEAR:   return (this->datetime[section] + 1) % 99;
    case DT_MONTH:  return (this->datetime[section] + 1) % 13;
    case DT_DAY:    return (this->datetime[section] + 1) % 32;
    case DT_HOUR:   return (this->datetime[section] + 1) % 24;
    case DT_MINUTE: return (this->datetime[section] + 1) % 60;
    case DT_SECOND: return (this->datetime[section] + 1) % 60;
#else
    case DT_YEAR:   return rtc.getYear();
    case DT_MONTH:  return rtc.getMonth();
    case DT_DAY:    return rtc.getDay();
    case DT_HOUR:   return rtc.getHours();
    case DT_MINUTE: return rtc.getMinutes();
    case DT_SECOND: return rtc.getSeconds();
    default:     return 99;
#endif
  }
}

// Update all date and time values
void Time::update_time(bool force, int section) {

  // Stop if we are past the Second position
  if (section < 0 || section >= DT_SECTIONS) { return; }

  this->datetime[section] = this->get_rtc(section);

  if (force || this->datetime[section] == 0) {
    this->update_time(force, section - 1);
  }
}

// Compares two datetimes.
// returns 1 if the first is later than the last
// returns 0 if they're equal
// returns -1 if the last is later than the first
int Time::dtcmp(DateTime first, DateTime last) {
  int ret = 0;
  for (int i=0; i<DT_SECOND+1; i++) {
    if (first[i] > last[i]) return 1;
    if (first[i] != last[i]) ret = -1;
  }
  return ret;
}

// Calculate day of week
byte Time::get_dow(DateTime datetime) {
  byte month = datetime[DT_MONTH];
  byte year  = datetime[DT_YEAR];
  return (datetime[DT_DAY]
        + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5)
        + (365 * (year + 4800 - ((14 - month) / 12)))
        + ((year + 4800 - ((14 - month) / 12)) / 4)
        - ((year + 4800 - ((14 - month) / 12)) / 100)
        + ((year + 4800 - ((14 - month) / 12)) / 400)
        - 32045
      ) % 7;

}
