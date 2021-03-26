#include <STM32IOLib.h>
STM32IO stm;
void buttonChangedCallback(uint8_t n,uint8_t s);
void switchChangedCallback(uint8_t n,uint8_t s);
void relayChangedCallback(uint8_t n,uint8_t s);
void setup(){
  Serial.begin(115200);
  Serial.print("\nInitializing...");
  stm.begin(true);
  delay(100);  
  Serial.println("Done.");
  delay(1000);
  stm.onButtonStateChanged(buttonChangedCallback);
  stm.onSwitchStateChanged(switchChangedCallback);
  stm.onRelayStateChanged(relayChangedCallback);
}
void loop() {
  stm.loop();
}

void buttonChangedCallback(uint8_t n,uint8_t s){
  Serial.printf("Button %u => %s\n",n,s?"PUSH":"RELEASED");
}

void switchChangedCallback(uint8_t n,uint8_t s){
  Serial.printf("Switch %u => %s\n",n,s?"PUSH":"RELEASED");
}

void relayChangedCallback(uint8_t n,uint8_t s){
  Serial.printf("Relay %u => %s\n",n,s?"ON":"OFF");
}

