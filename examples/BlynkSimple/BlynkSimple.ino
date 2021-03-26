
//Blynk Clone QR: https://github.com/iamdev/ESP8266-STM32IO-4CH-LCD/blob/main/blynk_clone_1877332670.png

#define BLYNK_MSG_LIMIT 250
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <STM32IOLib.h>
STM32IO stm;
LiquidCrystalSTM lcd(&stm);

void relayChangedCallback(uint8_t n,uint8_t s);
void buttonChangedCallback(uint8_t n,uint8_t s);
void updateClock(void);
void updateBlynkAnalog(void);

char blynk_server[] = "blynk-cloud.com";
int blynk_port = 80;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char blynk_auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char wifi_ssid[] = "YourNetworkName";
char wifi_pass[] = "YourPassword";

void setup()
{
  Serial.begin(115200);
  Serial.print("\nInitializing...");
  stm.begin(true);
  delay(100);
  Serial.println("Done.");
  
  lcd.backlight(255);
  lcd.clear();
  lcd.setCursor(0,0);
  
  stm.onRelayStateChanged(relayChangedCallback);
  stm.onButtonStateChanged(buttonChangedCallback);
  
  WiFi.begin(wifi_ssid, wifi_pass);
  Blynk.config(blynk_auth,blynk_server,blynk_port);
  Task.create([]{stm.loop();},10);
  Task.create(updateClock,1000);
  Task.create(updateBlynkAnalog,100);
}

void loop()
{
  if(WiFi.status() == WL_CONNECTED){
    Blynk.run();
  }
  Task.loop();
}

void relayChangedCallback(uint8_t n,uint8_t s){
  Serial.printf("Relay %u => %s\n",n,s?"ON":"OFF");
  Blynk.virtualWrite(V1+n-1,s);
}

void buttonChangedCallback(uint8_t n,uint8_t s){
  Serial.printf("Button %u => %s\n",n,s?"PUSH":"RELEASED");
  Blynk.virtualWrite(V10+n,s?255:0);
}

void updateClock(void){
  char buf[17];
  DateTime dt(now());
  snprintf(buf,17,"%02u/%02u/%04u %02u:%02u",dt.day,dt.month,dt.year,dt.hour,dt.minute);
  lcd.setCursor(0,0);
  lcd.print(buf);
}

void updateBlynkAnalog(void){
  static float a_temp[4];
  for(int i=0;i<4;i++){
    float a = stm.analogRead(i);
    if(abs((a-a_temp[i])*100)>=1){
      a_temp[i] = a;
      Blynk.virtualWrite(V6+i,a); 
    }    
  }
}

#define BLYNK_WRITE_RELAY(n) BLYNK_WRITE(V##n){ \
  stm.relayWrite(n,param.asInt());\
}
BLYNK_WRITE_RELAY(1);
BLYNK_WRITE_RELAY(2);
BLYNK_WRITE_RELAY(3);
BLYNK_WRITE_RELAY(4);

BLYNK_WRITE(V5){ \
  if(param.asInt())
    stm.buzzerOn();
  else
    stm.buzzerOff();
}

