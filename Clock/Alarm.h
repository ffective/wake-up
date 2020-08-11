#ifndef _ALARM_H_GUARD_20200810_
#define _ALARM_H_GUARD_20200810_

#include <Arduino.h>
#include "Time.h"

//Debug and development
#define _DEBUG_POPULATE_ALARM_LIST // Create some entries to test the alarm functionality.

#define ALM_MONDAY 0
#define ALM_TUESDAY ALM_MONDAY+1
#define ALM_WEDNESDAY ALM_TUESDAY+1
#define ALM_THURSDAY ALM_WEDNESDAY+1
#define ALM_FRIDAY ALM_THURSDAY+1
#define ALM_SATURDAY ALM_FRIDAY+1
#define ALM_SUNDAY ALM_SATURDAY+1
#define ALM_ENTRIES 7
#define ALM_HOUR 0
#define ALM_MINUTE ALM_HOUR+1
#define ALM_STATE ALM_MINUTE+1

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
