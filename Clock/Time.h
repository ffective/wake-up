#ifndef _TIME_H_GUARD_20200810_
#define _TIME_H_GUARD_20200810_

#include <Arduino.h>
#include <RTCZero.h>

//Debug and development
#define _DEBUG_MOCK_RTC // Use this to run this with mocked values for time. The clock will increment as fast as the main loop hits update.

// Clock
#define DT_YEAR 0
#define DT_MONTH DT_YEAR+1
#define DT_DAY DT_MONTH+1
#define DT_HOUR DT_DAY+1
#define DT_MINUTE DT_HOUR+1
#define DT_SECOND DT_MINUTE+1
#define DT_SECTIONS DT_SECOND+1

typedef byte DateTime[DT_SECTIONS];

class Time {

  public:
    DateTime datetime;
    static int dtcmp(DateTime first, DateTime last);
    static byte get_dow(DateTime datetime);
    void update_time(bool force = false, int section = DT_SECOND);
    Time();

  private:
    RTCZero rtc;
    byte get_rtc(int section);
};

#endif
