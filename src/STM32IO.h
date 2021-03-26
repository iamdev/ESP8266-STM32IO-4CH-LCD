#ifndef _STM32IO_LIB_H
#define _STM32IO_LIB_H
#include "Arduino.h"
#define DEBUG

enum ExpCommands {
  EXP_CMD_NO_COMMAND       ,
  EXP_CMD_READY            ,
  EXP_CMD_RELAY_TOGGLE     ,
  EXP_CMD_RELAY_STATE      ,
  EXP_CMD_RELAY_STATE_DELAY,
  EXP_CMD_SW_MODE          ,
  EXP_CMD_SW_READ          ,
  EXP_CMD_INPUT_STATE      ,
  EXP_CMD_ANALOG_READ      ,
  EXP_CMD_BUZZER_ON        ,
  EXP_CMD_BUZZER_OFF       ,
  EXP_CMD_GET_TIME         ,
  EXP_CMD_SET_TIME         ,
  EXP_CMD_EEPROM_READ      ,
  EXP_CMD_EEPROM_WRITE     ,
  EXP_CMD_LCD_COMMAND      ,  
  EXP_CMD_LCD_WRITE        ,  
  EXP_CMD_LCD_SETCURSOR    ,  
  EXP_CMD_LCD_BACKLIGHT    ,
  EXP_CMD_LCD_HOME         ,
  EXP_CMD_LCD_CLEAR        ,
  EXP_CMD_LCD_DISPLAY      ,
  EXP_CMD_LCD_NODISPLAY    ,
  EXP_CMD_LCD_BLINK        ,
  EXP_CMD_LCD_NOBLINK      ,
  EXP_CMD_LCD_CURSOR       ,
  EXP_CMD_LCD_NOCURSOR     ,
  EXP_CMD_LCD_SCRL_LEFT    ,
  EXP_CMD_LCD_SCRL_RIGHT   ,
  EXP_CMD_LCD_RTL          ,
  EXP_CMD_LCD_LTR          ,
  EXP_CMD_LCD_AUTO_SCRL    ,
  EXP_CMD_LCD_NO_AUTO_SCRL ,
  EXP_CMD_LCD_CREATE_CHAR  ,
};

enum RelaySwitchModes{
 SW_MODE_NONE        = 0,
 SW_MODE_SWITCH      = 1,
 SW_MODE_TOGGLE      = 2,
 SW_MODE_UNDEFINED   = 3
};
#define RELAY_ON      1
#define RELAY_OFF     0

class STM32IO{
    public:
        STM32IO(){}
        void begin(bool waitready=false);      
        uint8_t sendCommand(ExpCommands cmd,const uint8_t*data,size_t len,uint32_t *ret=NULL);
        uint8_t sendCommand(ExpCommands cmd,uint32_t *ret=NULL);
        int ready(void);
        void loop();      
        void setTime(time_t tt);        
        time_t getTime();        
        uint8_t relayToggle(uint8_t n);
        uint8_t relayState(uint8_t n);        
        uint8_t relayState(uint8_t n,uint8_t state);
        void relayStateDelay(uint8_t n,uint8_t state,uint32_t delay_ms);
        uint8_t relayRead(uint8_t n);        
        uint8_t relayWrite(uint8_t n,uint8_t state);
        void relayWriteDelay(uint8_t n,uint8_t state,uint32_t delay_ms);
        void buzzerOn(uint16_t ms);
        void buzzerOn();
        void buzzerOff();
        float analogRead(uint8_t n);

        uint8_t eepromRead(uint16_t addr);
        uint8_t eepromRead(uint16_t addr,uint8_t *buffer,uint8_t len);
        uint16_t eepromRead16(uint16_t addr);
        uint32_t eepromRead32(uint16_t addr);
        float eepromReadFloat(uint16_t addr);

        void eepromWrite(uint16_t addr,uint8_t data);
        void eepromWrite(uint16_t addr,const uint8_t * data, uint8_t len);
        void eepromWrite16(uint16_t addr,uint16_t data);
        void eepromWrite32(uint16_t addr,uint32_t data);
        void eepromWriteFloat(uint16_t addr,float data);

        void switchMode(uint8_t n,RelaySwitchModes mode);
        RelaySwitchModes switchMode(uint8_t n);        
        uint8_t buttonState(uint8_t n);
        uint8_t swtichState(uint8_t n);

        void onButtonStateChanged(void (*callback)(uint8_t,uint8_t));   
        void onSwitchStateChanged(void (*callback)(uint8_t,uint8_t));   
        void onRelayStateChanged(void (*callback)(uint8_t,uint8_t));   

        void lcd_backlight(uint8_t level);
        uint8_t lcd_backlight();
        void lcd_command(uint8_t cmd);
        void lcd_write(const uint8_t*buf,uint8_t len);
        void lcd_write(uint8_t buf);
        void lcd_setCursor(uint8_t col,uint8_t row);     
        void lcd_clear();
        void lcd_home();
        void lcd_noDisplay();
        void lcd_display();
        void lcd_noBlink();
        void lcd_blink();
        void lcd_noCursor();
        void lcd_cursor();
        void lcd_scrollDisplayLeft();
        void lcd_scrollDisplayRight();
        void lcd_leftToRight();
        void lcd_rightToLeft();
        void lcd_autoscroll();
        void lcd_noAutoscroll();
        void lcd_createChar(uint8_t, uint8_t[]); 

    protected :    
        void readInput();
        void (*buttonStateChangeCallback)(uint8_t,uint8_t) = NULL;
        void (*switchStateChangeCallback)(uint8_t,uint8_t) = NULL;
        void (*relayStateChangeCallback)(uint8_t,uint8_t) = NULL;
 
};
extern STM32IO stm;
#endif

