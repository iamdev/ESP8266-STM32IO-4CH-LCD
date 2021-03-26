#include "LEDBlink.h"
#include <stdio.h>
#include <stdarg.h>

LEDBlink::LEDBlink(uint8_t pin,int onState):
     _ledPin(pin),_onState(onState)
{
}
void LEDBlink::begin(){
    pinMode(_ledPin,OUTPUT);
    digitalWrite(_ledPin,!_onState);
}
void LEDBlink::on(){
    enable(false);
    digitalWrite(_ledPin,_onState);
}
void LEDBlink::off(){
    enable(false);
    digitalWrite(_ledPin,!_onState);
}
void LEDBlink::loop(){
    if(_isEnabled){
        unsigned long ms = (millis() - _offset_ms)%_period_ms  ;
        if(_currentStep >=_step && ms<_pattern[1]){
          _currentStep = 0;
          _nextState_ms = 0;
          _nextState = _onState;
        }
        if(ms >= _nextState_ms && _state != _nextState){
          _state = _nextState;
          digitalWrite(_ledPin,_state);
          _nextState = !_state;
          _currentStep++;
          _nextState_ms = _pattern[_currentStep];                   
        }        
    }
}

#define LIMIT(n,a,b) (n<a?a:n>b?b:n)
void LEDBlink::setPattern(const int n,...){
    va_list args;
    va_start(args, n);
    int c = 0;
    int p = 0;
    _pattern[c++] = 0;
    for(int i=0;i<n;i++){
        int v = va_arg(args,int);
        v = LIMIT(v,0,30000);
        p+=v;
        _pattern[c++] = p;
    }    
    va_end(args);
    
    _step = c-1;
    _period_ms = p;
    _currentStep = 0;
    _offset_ms = millis();
    _nextState_ms = _pattern[0];
    _nextState = _onState;    
    _state = !_onState;
    enable();
    loop();    
}

void LEDBlink::setPeriod(int period,int on_ms){   
  setPattern(2,on_ms,period-on_ms);     
}

void LEDBlink::enable(bool en){
    _isEnabled = en;    
}
