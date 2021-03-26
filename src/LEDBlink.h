#ifndef _LEDBlink_H
#define _LEDBlink_H
#include <Arduino.h>

class LEDBlink{
    public:
        LEDBlink(uint8_t pin,int onState=LOW);        
        void begin();
        void loop();
        void setPeriod(int period,int on_ms);
        void setPattern(const int n,...);
        void enable(bool en=true);            
        bool isEnabled() const {return _isEnabled;}
        void on();
        void off();
    private:
        uint8_t _ledPin;
        bool _isEnabled;
        uint16_t _pattern[32];
        int _currentStep = 0;
        int _step=0;
        int _onState;
        int _period_ms = 1000;
        int _offset_ms = 0;
        bool _state;
        bool _nextState;
        uint16_t _nextState_ms;

};

#endif /*_LEDBlink_H*/

