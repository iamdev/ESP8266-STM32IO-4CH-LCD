//Display on Serial Plotter
#include <STM32IOLib.h>
STM32IO stm;
void setup() {
  Serial.begin(115200);
  stm.begin(true);  
  Serial.println("\nReady!");
}

void loop() {   
  for (int i=0;i<4;i++){
    float a = stm.analogRead(i);        
    Serial.printf("A%u:%0.3f,",i,a);
  }
  Serial.println();
  delay(1000);
}