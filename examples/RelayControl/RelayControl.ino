//Display on Serial Plotter
#include <STM32IOLib.h>
STM32IO stm;
void setup() {
  Serial.begin(115200);
  stm.begin(true);  
  Serial.println("\nReady!");

  Serial.println("1.Set Relay 1 ON");
  stm.relayWrite(1,1);
  delay(1000);

  Serial.println("2.Set Relay 1 OFF");
  stm.relayWrite(1,0);
  delay(3000);

  Serial.println("3.Toggle Relay 2 to time.");
  stm.relayToggle(2); //toggle to ON
  delay(1000);
  stm.relayToggle(2); //toggle to OFF
  delay(3000);

  Serial.println("4.Set Relay 1 ON immediately and Relay 1 automatic OFF after 1 second.");
  stm.relayWriteDelay(1,1,1000); 
  stm.relayWrite(2,1); 
  delay(3000);

  Serial.print("5.1 Read status of Relay 1 : ");
  Serial.println(stm.relayRead(1)?"ON":"OFF");
  Serial.print("5.2 Read status of Relay 2 : ");
  Serial.println(stm.relayRead(2)?"ON":"OFF");
  delay(3000);
  
  Serial.print("6. Test Delay ON all channel.");
  stm.relayWriteDelay(1,1,1000);
  stm.relayWriteDelay(2,1,2000);
  stm.relayWriteDelay(3,1,3000);
  stm.relayWriteDelay(4,1,4000);
}

void loop() {   

}