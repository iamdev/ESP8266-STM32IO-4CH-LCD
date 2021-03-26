//Display on Serial Plotter
#include <STM32IOLib.h>
STM32IO stm;
LiquidCrystalSTM lcd(&stm);
void setup() {
  Serial.begin(115200);
  stm.begin(true);  
  Serial.println("\nReady!");

  for(int i=0;i<256;i+=5){
    lcd.backlight(i);
    delay(5);
  }
  lcd.backlight(255);
  lcd.setCursor(0,0);
  lcd.write("Test");
  lcd.setCursor(5,1);
  lcd.write("123456");
}

void loop() {   
  if(Serial.available()){
    switch(Serial.read()){
      case 'h':lcd.home(); break;
      case 'x':lcd.clear(); break;
      case 'd':lcd.display();break;
      case 'D':lcd.noDisplay();break;
      case 'b':lcd.blink();break;
      case 'B':lcd.noBlink();break;
      case 'c':lcd.cursor();break;
      case 'C':lcd.noCursor();break;
      case 'r':lcd.rightToLeft();break;
      case 'l':lcd.leftToRight();break;
      case 'L':lcd.scrollDisplayLeft();break;
      case 'R':lcd.scrollDisplayRight();break;
      case 'a':lcd.autoscroll();break;
      case 'A':lcd.noAutoscroll();break;
    }
  }
}