#include <Arduino.h>
#include <U8x8lib.h>
//#include <RTCZero.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#define X 128
#define Y 64

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// +1 because we need '\0' at the end of the string
char alm[X/8+1];      //Alarm font is standard size

// Time and date
#define YEAR 0
#define MONTH 1
#define DAY 2
#define HOUR 3
#define MINUTE 4
#define SECOND 5
byte clk[6] = {98, 12, 31, 23, 59, 45};

void setup(void)
{
  // Initialize display
  u8x8.begin();

  // Initialize clock
  //rtc.begin();
}

// Prints the main clock section of the display
void print_clock(byte* datetime) {
  u8x8.setCursor(0,3);
  u8x8.setFont(u8x8_font_profont29_2x3_n);
  print_2digit(datetime[HOUR]);
  u8x8.print(":");
  print_2digit(datetime[MINUTE]);
  u8x8.print(":");
  print_2digit(datetime[SECOND]);
}

// Prints the date section of the display
void print_date(byte* datetime) {
  u8x8.setCursor(4,7);
  u8x8.setFont(u8x8_font_saikyosansbold8_n);
  print_2digit(datetime[DAY]);
  u8x8.print("/");
  print_2digit(datetime[MONTH]);
  u8x8.print("/");
  print_2digit(datetime[YEAR]);
}

// Prints a section to the RTC with a leading zero if neSECONDcessary
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

// Update all date and time values
void update_time(byte datetime[], int len, int section = SECOND) {

  // Stop if we are past the Second position
  if (section < 0 || section >= len) { return; }

  datetime[section] = get_rtc(section);

  if (datetime[section] == 0) {
    update_time(datetime, len, section - 1);
  }
}


// Get the relevant information from the RTC
// Temporarily accessing global clk to test until MKRZero arrives.
byte get_rtc(int section) {
  switch (section) {
    case YEAR:
      return (clk[section] + 1) % 99;
      //return rtc.getYear();
    case MONTH:
      return (clk[section] + 1) % 13;
      //return rtc.getMonth();
    case DAY:
      return (clk[section] + 1) % 32;
      //return rtc.getDay();
    case HOUR:
      return (clk[section] + 1) % 24;
      //return rtc.getHours();
    case MINUTE:
      return (clk[section] + 1) % 60;
      //return rtc.getMinutes();
    case SECOND:
      return (clk[section] + 1) % 60;
      //return rtc.getSeconds();
    default:
      return 99;
  }
}

void loop(void)
{
  //u8x8.clearDisplay();

  //u8x8.setPowerSave(0);
  
  // Alarm indication
  // TODO - form this into a function
  u8x8.setFont(u8x8_font_open_iconic_embedded_1x1);
  u8x8.drawGlyph(0, 0, '@'+1);
  u8x8.setFont(u8x8_font_saikyosansbold8_u);
  // TODO - implement array for alarm dow
  u8x8.drawString(1, 0, " 08:13  MDWDV..");
  
  // Main clock
  update_time(clk, sizeof(clk));
  print_clock(clk);
  print_date(clk);

  delay(300);
}
