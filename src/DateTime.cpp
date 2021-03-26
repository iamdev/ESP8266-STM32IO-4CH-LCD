#include "DateTime.h"
//********************************************************************/
// Implement DateTime
//********************************************************************/
DateTime::DateTime (time_t t) {
    static int daysPerMonths[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int y = 0;
    int m =0;
    int d =0;
    int ss = t % 60;
    t /= 60;
    int mm = t % 60;
    t /= 60;
    int hh = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;
    for (y = 0; ; ++y) {
        leap = (y+2) % 4 == 0;
        if (days < 365 + leap)
            break;
        days -= 365 + leap;
    }
    for (m = 1; ; ++m) {
        uint8_t daysPerMonth = daysPerMonths[m-1];
        if (leap && m == 2)
            ++daysPerMonth;
        if (days < daysPerMonth)
            break;
        days -= daysPerMonth;
    }
    d = days+1; 
    year = y+1970;
    month = m;
    day = d;
    hour = hh>23?0:hh;
    minute = mm>59?0:mm;
    second = ss>59?0:ss;
}

DateTime::DateTime (uint16_t y, uint8_t m, uint8_t d, uint8_t hh, uint8_t mm, uint8_t ss) {
    year = y;
    month = m;
    day = d;
    hour = hh;
    minute = mm;
    second = ss;
}

DateTime::DateTime (const char* datetime)
{
    uint8_t buf[6] = {1970,1,1,0,0,0};
    char str[32];
    strncpy(str,datetime,32);
    char del[] = ":/- ";
    char * tok= strtok(str,del);
    int i = 0;
    bool yearFirst = false;
    bool monthFirstStr = false;
    while(i<6 && tok){      
        if(i==0 && datetime[strlen(tok)] == ':')i=3;
        int v=0;
        if( i==0 && tok[0]>='A' && tok[0]<='Z'){
          String monthStr = String(tok);
          static const char* strMonths[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
          v=1;
          for(int m=0;m<12;m++){              
              if(monthStr.equals(strMonths[m])){
                  v = m+1; 
                  break;
              } 
          }          
          monthFirstStr = true;         
        }
        else
        {
          v = atol(tok);            
          if(i==0 && v>99) yearFirst = true;
          if(v>99)v=v%100;
        }
        buf[i++] = v;    
        tok = strtok(NULL,del);
    }
    if(monthFirstStr){
        this->month = buf[0];
        this->day = buf[1];
        this->year = buf[2]+2000;    
    }else
    if(yearFirst){
        this->year = buf[0]+2000;
        this->month = buf[1];
        this->day = buf[2];
    }else{
        this->year = buf[2]+2000;
        this->month = buf[1];
        this->day = buf[0];    
    }
    this->hour = buf[3];
    this->minute = buf[4];
    this->second = buf[5];

}

byte DateTime::dayOfWeek() {   // y > 1752, 1 <= m <= 12
  int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};  
  year -= month < 3;
  return ((year + year/4 - year/100 + year/400 + t[month-1] + day) % 7) + 1; // 01 - 07, 01 = Sunday
}

time_t DateTime::timestamp(){
    static int monthdays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	if(year<1970) return 0;
    int yd = year-1970;
    long days = ((yd)*365) + ((yd+2)/4) + (day-1);
    for(int i = 0;i<month;i++) days+=monthdays[i];
    if(((yd+2)%4) == 0 && month<=2) days -=1;
    return (time_t) (((days * 24L + hour) * 60 + minute) * 60 + second);
}

DateTime:: operator time_t(){
  return timestamp();
}
//********************************************************************/
