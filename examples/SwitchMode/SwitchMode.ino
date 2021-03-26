#include <STM32IOLib.h>
STM32IO stm;
void switchChangedCallback(uint8_t n,uint8_t s);
void setup(){
  Serial.begin(115200);
  Serial.print("\nInitializing...");
  stm.begin(true);
  delay(100);  
  Serial.println("Done.");

  stm.switchMode(1,SW_MODE_TOGGLE);
  stm.switchMode(2,SW_MODE_SWITCH);
  stm.switchMode(3,SW_MODE_NONE);
  stm.switchMode(4,SW_MODE_NONE);
  stm.onSwitchStateChanged(switchChangedCallback);  
}
int btn1_state = 0;
void loop() {  
  stm.loop();
}

void switchChangedCallback(uint8_t n,uint8_t s){
  if(n==3 && !s){
    stm.relayToggle(3);
    stm.relayStateDelay(4,RELAY_ON,2000);
  } 
}
