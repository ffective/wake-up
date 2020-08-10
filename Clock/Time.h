#ifndef _TIME_H_GUARD_20200810_
#define _TIME_H_GUARD_20200810_

#include <Arduino.h>
#include <RTCZero.h>

//Debug and development
#define _DEBUG_MOCK_RTC // Use this to run this with mocked values for time. The clock will increment as fast as the main loop hits update.

// Clock
#define YEAR 0
#define MONTH YEAR+1
#define DAY MONTH+1
#define HOUR DAY+1
#define MINUTE HOUR+1
#define SECOND MINUTE+1
#define SECTIONS SECOND+1

typedef byte DateTime[SECTIONS];

class Time {

  public:
    DateTime datetime;
    void update_time(bool force = false, int section = SECOND);
    Time();

  private:
    RTCZero rtc;
    byte get_rtc(int section);
};

#endif
