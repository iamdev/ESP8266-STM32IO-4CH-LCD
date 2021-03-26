#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <STM32IOLib.h>
STM32IO stm;

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char * ssid = STASSID; // your network SSID (name)
const char * pass = STAPSK;  // your network password
const char* ntpServerName = "time.nist.gov";
unsigned int ntpLocalPort = 2390;

//NtpServer ntp(ntpServerName,ntpLocalPort);
NtpServer ntp;
bool isSync = false;
void setup() {
    Serial.begin(115200);
    Serial.print("\nInitializing...");
    stm.begin(true);
    Serial.println("Done.");
    delay(1000);
    time_t tt= stm.getTime();
    DateTime dt(tt);
    Serial.printf("Current local time : %02u/%02u/%04u %02u:%02u:%02u\n",dt.day,dt.month,dt.year,dt.hour,dt.minute,dt.second);
    
    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    ntp.begin();
}

void loop() {
    if(!isSync && WiFi.status() == WL_CONNECTED){
        Serial.println("Starting get time from server...");        
        time_t tt = ntp.getTime(); 
        DateTime dt = DateTime(tt);
        if(dt.year>=2020){
            Serial.printf("Server Time : %02u/%02u/%04u %02u:%02u:%02u\n",dt.day,dt.month,dt.year,dt.hour,dt.minute,dt.second);
            stm.setTime(tt);    
            isSync = true;
            Serial.println("Sync server time success!");
        }else{
            Serial.println("Failed to get time from server");
        }
    }
    
    DateTime dt = DateTime(stm.getTime());
    Serial.printf("%02u/%02u/%04u %02u:%02u:%02u\n",dt.day,dt.month,dt.year,dt.hour,dt.minute,dt.second);

    delay(1000);
}