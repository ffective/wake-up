#include <Arduino.h>

//Own code
#include "Time.h"
#include "Alarm.h"
#include "Display.h"

Time clk;
Alarm alm;
Display disp;

void setup(void) {}

void loop(void)
{
  //u8x8.clearDisplay();

  //u8x8.setPowerSave(0);

  clk.update_time();

  // Alarm indication
  disp.print_alarm(alm.alarms, clk.datetime);

  // Main clock
  disp.print_clock(clk.datetime);
  disp.print_date(clk.datetime);

  delay(300);
}
