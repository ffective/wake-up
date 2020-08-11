#include "Alarm.h"

Alarm::Alarm(void) {
#ifdef _DEBUG_POPULATE_ALARM_LIST
  this->alarms[ALM_MONDAY]    = { 1,  2,  true };
  this->alarms[ALM_TUESDAY]   = { 3,  4,  false };
  this->alarms[ALM_WEDNESDAY] = { 5,  6,  true };
  this->alarms[ALM_THURSDAY]  = { 7,  8,  false };
  this->alarms[ALM_FRIDAY]    = { 9,  10, true };
  this->alarms[ALM_SATURDAY]  = { 11, 12, false };
  this->alarms[ALM_SUNDAY]    = { 13, 14, true };
#endif
}

// Returns the index of the next enabled alarm based on the given DateTime.
// Returns -1 (underflow) if there is no next enabled alarm.
byte Alarm::next_alarm(DateTime dt) {
  byte dow = Time::get_dow(dt);
  byte index = dow;
  byte i = 0;
  for (i; i<ALM_ENTRIES; i++) {
    index = i + dow % ALM_ENTRIES;

    // Only looking for enabled alarms
    if (! this->alarms[index].enabled) continue;

    //https://arduino.stackexchange.com/questions/44585/what-is-causing-this-narrowing-conversion-warning
    byte alarmdt[] = {dt[DT_YEAR], dt[DT_MONTH], (byte)(dt[DT_DAY] + i),
                      this->alarms[index].hour, this->alarms[index].minute, 0};
    if (Time::dtcmp(alarmdt, dt) >= 0) break;
  }
  if (i == 8) return -1; // underflow on "error"
  return index;
}
