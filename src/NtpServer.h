#ifndef _NtpServer_H
#define _NtpServer_H
#include <Arduino.h>
#include <WiFiUdp.h>
#ifdef ARDUNO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif ARDUNO_ARCH_ESP32
#include <WiFi.h>
#endif

#define NtpServer_DEFAULT_PORT      123
#define NtpServer_DEFAULT_SERVER    "time.nist.gov"
#define NTP_PACKET_SIZE             48
#define NtpServer_DEFAULT_TIMEOUT   5000
#define NTP_DEFAULT_TIMEZONE        7

class NtpServer{
    public :
        NtpServer();
        NtpServer(const char * ntpServer,int localport=NtpServer_DEFAULT_PORT);
        void begin();
        time_t getTime(int timezone=NTP_DEFAULT_TIMEZONE,int timeout=NtpServer_DEFAULT_TIMEOUT);
    private:
        boolean sendNTPpacket ();
        char _ntpServer[100];
        int _ntpLocalPort;
        WiFiUDP *udp;
        int timeZone=NTP_DEFAULT_TIMEZONE;
};
#endif
