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
