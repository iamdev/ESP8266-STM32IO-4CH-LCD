#ifndef  _DATETIME_H
#define  _DATETIME_H
#include <Arduino.h>
class DateTime {
  public:
      DateTime (time_t);
      DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour =0, uint8_t min =0, uint8_t sec =0);
      DateTime (const char* datetime);
      uint16_t year;
      uint8_t month;
      uint8_t day;
      uint8_t hour;
      uint8_t minute;
      uint8_t second;
      time_t timestamp();
      uint8_t dayOfWeek();
      operator time_t();    
};
extern time_t now();
#endif