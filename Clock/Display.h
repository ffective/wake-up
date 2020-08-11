#ifndef _DISPLAY_H_GUARD_20200811_
#define _DISPLAY_H_GUARD_20200811_

#include <Arduino.h>

#include "Alarm.h"
#include "Time.h"

#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

class Display {

  public:
    // constructor
    Display();
    // methods
    void print_alarm(Alarm_entry* alarms, byte* datetime);
    void print_clock(byte* datetime);
    void print_date(byte* datetime);

  private:
    // vars
    // methods
    const char* dow_str(int dow);
    void print_2digit(byte number);
};

#endif
