#include "LiquidCrystalSTM.h"
#include "STM32IOLib.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystalSTM constructor is called).

LiquidCrystalSTM::LiquidCrystalSTM(STM32IO*stm) : _stm(stm)
{
}

void LiquidCrystalSTM::begin()
{

}

/********** high level commands, for the user! */
void LiquidCrystalSTM::clear()
{
  _stm->lcd_clear();
}

void LiquidCrystalSTM::home()
{
  _stm->lcd_home();
}

void LiquidCrystalSTM::setCursor(uint8_t col, uint8_t row)
{  
  _stm->lcd_setCursor(col,row);
}

// Turn the display on/off (quickly)
void LiquidCrystalSTM::noDisplay() {
  _stm->lcd_noDisplay();
}
void LiquidCrystalSTM::display() {
  _stm->lcd_display();
}

// Turns the underline cursor on/off
void LiquidCrystalSTM::noCursor() {
  _stm->lcd_noCursor();
}
void LiquidCrystalSTM::cursor() {
  _stm->lcd_cursor();
}

// Turn on and off the blinking cursor
void LiquidCrystalSTM::noBlink() {
  _stm->lcd_noBlink();  
}
void LiquidCrystalSTM::blink() {
  _stm->lcd_blink();
}

// These commands scroll the display without changing the RAM
void LiquidCrystalSTM::scrollDisplayLeft(void) {
  _stm->lcd_scrollDisplayLeft();
}
void LiquidCrystalSTM::scrollDisplayRight(void) {
  _stm->lcd_scrollDisplayRight();
}

// This is for text that flows Left to Right
void LiquidCrystalSTM::leftToRight(void) {
  _stm->lcd_leftToRight();
}

// This is for text that flows Right to Left
void LiquidCrystalSTM::rightToLeft(void) {
  _stm->lcd_rightToLeft();
}

// This will 'right justify' text from the cursor
void LiquidCrystalSTM::autoscroll(void) {
  _stm->lcd_autoscroll();
}

// This will 'left justify' text from the cursor
void LiquidCrystalSTM::noAutoscroll(void) {
  _stm->lcd_noAutoscroll();
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystalSTM::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; 
  _stm->lcd_createChar(location,charmap);
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystalSTM::command(uint8_t value) {
  _stm->lcd_command(value);
}

inline size_t LiquidCrystalSTM::write(uint8_t value) {
  _stm->lcd_write(value);
  return 1; // assume sucess
}

uint8_t LiquidCrystalSTM::backlight(){
  return _stm->lcd_backlight();
}
void LiquidCrystalSTM::backlight(uint8_t value){
  _stm->lcd_backlight(value);
}
