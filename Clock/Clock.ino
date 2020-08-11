#include <Arduino.h>

//Display stuff
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);


//Own code
#include "Time.h"
#include "Alarm.h"

Time clk;
Alarm alm;

void setup(void)
{
  // Initialize display
  u8x8.begin();

  // Initialize clock
  //rtc.begin();
}


// Fill a buffer with a three-character string representing the DoW
const char* dow_str(int dow) {
  return ((const char *[]){"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"})[dow];
}

// Prints the main clock section of the display
void print_clock(byte* datetime) {
  u8x8.setFont(u8x8_font_profont29_2x3_n);

  u8x8.setCursor(0, 3);
  print_2digit(datetime[DT_HOUR]);
  u8x8.print(":");
  //TODO - make this easily selectable.
  //u8x8.setInverseFont(1);
  print_2digit(datetime[DT_MINUTE]);
  //u8x8.setInverseFont(0);
  u8x8.print(":");
  print_2digit(datetime[DT_SECOND]);
}

// Prints the date section of the display
void print_date(byte* datetime) {
  u8x8.setFont(u8x8_font_saikyosansbold8_u);

  //Print Day of Week
  u8x8.setCursor(0,7);
  u8x8.print(dow_str(Time::get_dow(datetime)));

  // Print date string
  char date_str[8];
  sprintf(date_str, "%02d/%02d/%02d", datetime[DT_DAY],
          datetime[DT_MONTH], datetime[DT_YEAR]);
  u8x8.drawString(8, 7, date_str);
}

// Prints the top line with alarm information.
void print_alarm(Alarm_entry* alarms, byte* datetime) {
  // Draw alarm symbol
  u8x8.setFont(u8x8_font_open_iconic_embedded_1x1);
  u8x8.drawGlyph(0, 0, '@'+1);

  // Get index of next alarm
  byte index = alm.next_alarm(datetime);
  // Draw alarm time
  u8x8.setFont(u8x8_font_saikyosansbold8_u);
  if (index < ALM_ENTRIES) {
    char alm_str[5];
    sprintf(alm_str, "%02d:%02d",
            alarms[index].hour, alarms[index].minute);
    u8x8.drawString(2, 0, alm_str);
  }
  else {
    u8x8.drawString(2, 0, "--:--");
  }

  for (int i=0; i<ALM_ENTRIES; i++) {
    //Draw alarm day representation
    u8x8.setCursor(9+i,0);
    if (alarms[i].enabled) {
      char dow[2] = {dow_str(i)[0], '\0'};
      u8x8.print(dow);
    }
    else {
      u8x8.print(".");
    }

    //Draw alarm day representation
    u8x8.setCursor(9+i,1);
    if (i == index) {
      u8x8.print("^");
    }
    else {
      u8x8.print(" ");
    }
  }
}

// Prints a section to the RTC with a leading zero if necessary
void print_2digit(byte number) {
  // Catch overflow
  if (number > 99) {
    number = 99;
  }

  // Convert to string
  char text[3];
  sprintf(text, "%02d", number);

  // Put it on the display
  u8x8.print(text);
}

void loop(void)
{
  //u8x8.clearDisplay();

  //u8x8.setPowerSave(0);

  clk.update_time();

  // Alarm indication
  print_alarm(alm.alarms, clk.datetime);

  // Main clock
  print_clock(clk.datetime);
  print_date(clk.datetime);

  delay(300);
}
