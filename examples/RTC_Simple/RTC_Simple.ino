#include <STM32IOLib.h>
STM32IO stm;
void setup() {
  Serial.begin(115200);
  Serial.print("\nInitializing...");
  uint32_t ms = millis();
  stm.begin(true);
  delay(1000);
  ms = millis()-ms;
  Serial.printf("Done.[%ums.]\n",ms);
  time_t tt= stm.getTime();
  Serial.printf("Current timestamp : %u\n",tt);
  if(tt <= 1577923200 || tt >= 4102444800){
    const char compile_date[] = __DATE__ " " __TIME__;
    DateTime dt(compile_date);
    time_t tt = ((time_t)dt)+13;
    dt = DateTime(tt);
    Serial.printf("Set complie time : %02u/%02u/%04u %02u:%02u:%02u [%u] \n",dt.day,dt.month,dt.year,dt.hour,dt.minute,dt.second,tt);
    stm.setTime(tt);
  }
}
void loop() {
  DateTime dt = DateTime(stm.getTime());
  Serial.printf("%02u/%02u/%04u %02u:%02u:%02u\n",dt.day,dt.month,dt.year,dt.hour,dt.minute,dt.second);
  delay(1000);
}