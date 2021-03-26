#include <STM32IOLib.h>
STM32IO stm;
void setup(){
  Serial.begin(115200);
  Serial.print("\nInitializing...");
  stm.begin(true);
  delay(100);  
  Serial.println("Done.");
}
int btn1_state = 0;
void loop() {  
  int btn1 = stm.buttonState(1);
  if(btn1!=btn1_state){
    Serial.print("Button 1 : ");
    Serial.println(btn1?"DOWN":"UP");
    btn1_state = btn1;
  }
  delay(10);
}
