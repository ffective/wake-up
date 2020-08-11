#ifndef _ALARM_H_GUARD_20200810_
#define _ALARM_H_GUARD_20200810_

#include <Arduino.h>
#include "Time.h"

//Debug and development
#define _DEBUG_POPULATE_ALARM_LIST // Create some entries to test the alarm functionality.

#define ALM_MONDAY    0
#define ALM_TUESDAY   1
#define ALM_WEDNESDAY 2
#define ALM_THURSDAY  3
#define ALM_FRIDAY    4
#define ALM_SATURDAY  5
#define ALM_SUNDAY    6
#define ALM_ENTRIES   7

typedef struct {
  byte hour;
  byte minute;
  bool enabled;
} Alarm_entry;

typedef Alarm_entry Alarm_list[ALM_ENTRIES];

class Alarm {

  public:
    Alarm_list alarms;
    Alarm();
    byte next_alarm(DateTime dt);

  private:
};

#endif
