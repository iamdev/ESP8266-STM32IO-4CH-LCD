#include <STM32IOLib.h>
STM32IO stm;
void setup() {
  Serial.begin(115200);  
  stm.begin(true);    
  Serial.println("\nReady!");
  stm.buzzerOn();
  delay(1000);
  stm.buzzerOff();
  delay(1000);
  stm.buzzerOn(100); //Auto off after 100ms.
}

void loop() {
  
}