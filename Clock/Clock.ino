#include <Arduino.h>
#include <U8x8lib.h>
//#include <RTCZero.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#define X 128
#define Y 64

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// Clock
#define YEAR 0
#define MONTH YEAR+1
#define DAY MONTH+1
#define HOUR DAY+1
#define MINUTE HOUR+1
#define SECOND MINUTE+1
byte clk[6] = {90, 11, 19, 5, 5, 45};

// Alarm
#define MONDAY 0
#define TUESDAY MONDAY+1
#define WEDNESDAY TUESDAY+1
#define THURSDAY WEDNESDAY+1
#define FRIDAY THURSDAY+1
#define SATURDAY FRIDAY+1
#define SUNDAY SATURDAY+1
#define ALM_HOUR 0
#define ALM_MINUTE ALM_HOUR+1
#define ALM_STATE ALM_MINUTE+1
#define ALM_ENTRIES 7
typedef struct {
  byte hour;
  byte minute;
  bool enabled;
} Alarm_entry;
Alarm_entry alm[ALM_ENTRIES] = { // Allow a different alarm for each day, and disabling it independently.
  { 1,  2,  true },//mon
  { 3,  4,  false },//tue
  { 5,  6,  true },//wed
  { 7,  8,  false },//thu
  { 9,  10, true },//fri
  { 11, 12, false },//sat
  { 13, 14, true }//sun
};

void setup(void)
{
  // Initialize display
  u8x8.begin();

  // Initialize clock
  //rtc.begin();
}

// Calculate day of week
byte get_dow(byte* datetime) {
  byte month = datetime[MONTH];
  byte year  = datetime[YEAR];
  return (datetime[DAY]
        + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5)
        + (365 * (year + 4800 - ((14 - month) / 12)))
        + ((year + 4800 - ((14 - month) / 12)) / 4)
        - ((year + 4800 - ((14 - month) / 12)) / 100)
        + ((year + 4800 - ((14 - month) / 12)) / 400)
        - 32045
      ) % 7;

}

// Fill a buffer with a three-character string representing the DoW
const char* dow_str(int dow) {
  return ((const char *[]){"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"})[dow];
}

// Prints the main clock section of the display
void print_clock(byte* datetime) {
  u8x8.setFont(u8x8_font_profont29_2x3_n);
  
  u8x8.setCursor(0, 3);
  print_2digit(datetime[HOUR]);
  u8x8.print(":");
  //TODO - make this easily selectable.
  //u8x8.setInverseFont(1);
  print_2digit(datetime[MINUTE]);
  //u8x8.setInverseFont(0);
  u8x8.print(":");
  print_2digit(datetime[SECOND]);
}

// Prints the date section of the display
void print_date(byte* datetime) {
  u8x8.setFont(u8x8_font_saikyosansbold8_u);

  //Print Day of Week
  u8x8.setCursor(0,7);
  u8x8.print(dow_str(get_dow(datetime)));

  // Print date string
  char date_str[8];
  sprintf(date_str, "%02d/%02d/%02d", datetime[DAY],
          datetime[MONTH], datetime[YEAR]);
  u8x8.drawString(8, 7, date_str);
}

// Compares two datetimes.
// returns 1 if the first is later than the last
// returns 0 if they're equal
// returns -1 if the last is later than the first
int dtcmp(byte* first, byte* last) {
  int ret = 0;
  for (int i=0; i<SECOND+1; i++) {
    if (first[i] > last[i]) return 1;
    if (first[i] != last[i]) ret = -1;
  }
  return ret;
}

// Returns the index of the next enabled alarm.
// Returns -1 (underflow) if there is no next enabled alarm.
byte next_alarm(Alarm_entry* alarms, byte* dt) {
  byte dow = get_dow(dt);
  byte index = dow;
  byte i = 0;
  for (i; i<ALM_ENTRIES; i++) {
    index = i + dow % ALM_ENTRIES;

    // Only looking for enabled alarms
    if (! alarms[index].enabled) continue;

    //https://arduino.stackexchange.com/questions/44585/what-is-causing-this-narrowing-conversion-warning
    byte alarmdt[] = {dt[YEAR], dt[MONTH], (byte)(dt[DAY] + i),
                      alarms[index].hour, alarms[index].minute, 0};
    if (dtcmp(alarmdt, dt) >= 0) break;
  }
  if (i == 8) return -1; // underflow on "error"
  return index;
}

// Prints the top line with alarm information.
void print_alarm(Alarm_entry* alarms, byte* datetime) {
  // Draw alarm symbol
  u8x8.setFont(u8x8_font_open_iconic_embedded_1x1);
  u8x8.drawGlyph(0, 0, '@'+1);

  // Get index of next alarm
  byte index = next_alarm(alarms, datetime);
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
// TODO - switch to commented-out statemens
byte get_rtc(int section) {
  switch (section) {
    case YEAR:   return (clk[section] + 1) % 99; //return rtc.getYear();
    case MONTH:  return (clk[section] + 1) % 13; //return rtc.getMonth();
    case DAY:    return (clk[section] + 1) % 32; //return rtc.getDay();
    case HOUR:   return (clk[section] + 1) % 24; //return rtc.getHours();
    case MINUTE: return (clk[section] + 1) % 60; //return rtc.getMinutes();
    case SECOND: return (clk[section] + 1) % 60; //return rtc.getSeconds();
    default:     return 99;
  }
}

void loop(void)
{
  //u8x8.clearDisplay();

  //u8x8.setPowerSave(0);

  update_time(clk, sizeof(clk));

  
  // Alarm indication
  print_alarm(alm, clk);
  
  // Main clock
  print_clock(clk);
  print_date(clk);

  delay(300);
}
