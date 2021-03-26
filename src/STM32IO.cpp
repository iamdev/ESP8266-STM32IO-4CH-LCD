#include "Arduino.h"
#include "SPI.h"
#include "STM32IO.h"
#include "DateTime.h"
#include "NtpServer.h"
#include <functional>
using namespace std;
//#define DEBUG_SPI_DATA
#define SPI_SS        15
#define SPI_FREQ      18000000
#define SPI_TRANSFER_DELAY ((uint16_t)((1e6f/SPI_FREQ)*1500))
#define SPI_RESPONSE_TIMEOUT_US 1000

float temp_analog[4] = {0.0f,0.0f,0.0f,0.0f};
uint8_t relay_states[4] = {0,0,0,0};

uint8_t getCRC8(const uint8_t *data,int length);
void STM32IO::begin(bool waitready)
{
  pinMode(SPI_SS, OUTPUT);
  digitalWrite(SPI_SS, HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));
  if(waitready){
    int d = 50;
    uint32_t ts = millis();
    delay(d);
    while(millis()-ts<5000 && !ready()){
      delay(d);      
      d+=50;
    }
  }
}

uint8_t STM32IO::sendCommand(ExpCommands cmd,uint32_t *ret)
{
  return sendCommand(cmd,NULL,0,ret);
}

uint32_t last_cmd_us = 0;
uint8_t STM32IO::sendCommand(ExpCommands cmd,const uint8_t*data,size_t len,uint32_t *ret){
  uint8_t tx_buf[32];
  uint8_t rx_buf[16];
  uint32_t ms = millis();
  uint8_t trx_id = ms&0xff;  
  uint8_t tx_len = len+4;
  tx_buf[0]=trx_id;
  tx_buf[1]=tx_len;
  tx_buf[2]=cmd;
  
  for(int i=0;i<len;i++){
    tx_buf[3+i] = data[i];
  }
  
  tx_buf[tx_len-1] = getCRC8(tx_buf,tx_len-1);
 
  #ifdef DEBUG_SPI_DATA
    if(len>0){
      for(int i=0;i<len;i++) Serial.printf("%02X ",tx_buf[i]);
      Serial.print(" ==> ");
    }  
  #endif
  
  while(micros()-last_cmd_us<1000){
    delayMicroseconds(10);
    yield();
  }
  
  GPOC = 1<<SPI_SS;
  for(int i=0;i<=(tx_len-1)/8;i++){
    SPI.writeBytes(tx_buf+(i*8),8);  
    delayMicroseconds(SPI_TRANSFER_DELAY);
  }
  GPOS = 1<<SPI_SS; 
  delayMicroseconds(10);
  GPOC = 1<<SPI_SS;
  tx_buf[0] = 0;
  tx_buf[1] = 0;
  uint32_t m = micros();
  int rx_index = 0;
  uint8_t res[6];
  bool isValidCrc = false;
  while(micros()-m<SPI_RESPONSE_TIMEOUT_US){        
    uint8_t*pbuf=rx_buf;    
    noInterrupts();
    SPI.transferBytes(tx_buf,rx_buf+rx_index,8);      
    interrupts();
    pbuf = rx_buf;
    for(int i=0;i<8;i++){
      if(pbuf[0] == trx_id)break;
      pbuf++;    
    }
    if(pbuf[0]== trx_id && pbuf[5] == getCRC8(pbuf,5)){
      for(int i=0;i<6;i++)res[i] = pbuf[i];
      isValidCrc = true;
      break;
    }
    if(rx_index==8)rx_index = 0; else rx_index = 8;
    
    yield();
    delayMicroseconds(SPI_TRANSFER_DELAY);     
  }
  GPOS = 1<<SPI_SS; 
  last_cmd_us = micros();
#ifdef DEBUG_SPI_DATA
  for(int i=0;i<6;i++) Serial.printf("%02X ",res[i]);  
#endif  
  if(!isValidCrc) {
#ifdef DEBUG_SPI_DATA
    Serial.print(" => Invalid CRC :");
    for(int i=0;i<16;i++) Serial.printf("%02X ",rx_buf[i]);  
#endif
    return 0;
  }
#ifdef DEBUG_SPI_DATA
  Serial.println();
#endif
  if(ret!=NULL) *ret=*((uint32_t*)(res+1));
  return 1;
}

int STM32IO::ready(void){
  uint32_t ret;
  return sendCommand(EXP_CMD_READY,&ret);
}

void STM32IO::buzzerOn(){
  buzzerOn(0xFFFF);
}

void STM32IO::buzzerOn(uint16_t ms){
  uint32_t ret;
  sendCommand(EXP_CMD_BUZZER_ON,(const uint8_t *)&ms,sizeof(ms),&ret);
}

void STM32IO::buzzerOff(){
  uint32_t ret;
  sendCommand(EXP_CMD_BUZZER_OFF,&ret);
}

float STM32IO::analogRead(uint8_t n){
  if(n<0||n>3)return 0;
  uint32_t ret;
  if(sendCommand(EXP_CMD_ANALOG_READ,&n,1,&ret)){
     //Serial.printf("[%08X]",ret);
     temp_analog[n] = *(float*)(&ret);
  }
  return temp_analog[n];
}

uint8_t STM32IO::relayToggle(uint8_t n){
  if(n<1||n>4)return 0;
  uint32_t ret;
  if(sendCommand(EXP_CMD_RELAY_TOGGLE,&n,1,&ret)) return ret?1:0;
  return relay_states[n-1];
}

uint8_t STM32IO::relayState(uint8_t n,uint8_t state){
  if(n<1||n>4)return 0;
  uint32_t ret;
  uint8_t buf[2] = {n,state};
  if(sendCommand(EXP_CMD_RELAY_STATE,buf,2,&ret)) {
    relay_states[n-1] = ret?1:0;
  }
  return relay_states[n-1];
}

uint8_t STM32IO::relayState(uint8_t n){
  if(n<1||n>4)return 0;
  uint32_t ret;
  if(sendCommand(EXP_CMD_RELAY_STATE,&n,1,&ret)) {
    relay_states[n-1] = ret?1:0;
  }
  return relay_states[n-1];
}

void STM32IO::relayStateDelay(uint8_t n,uint8_t state,uint32_t delay_ms){
  if(n<1||n>4)return;
  uint32_t ret;  
  uint8_t buf[6] = {n,state,};
  *((uint32_t*)(buf+2)) = delay_ms;
  sendCommand(EXP_CMD_RELAY_STATE_DELAY,buf,6,&ret);    
}


uint8_t STM32IO::relayRead(uint8_t n){
  return relayState(n);
}

uint8_t STM32IO::relayWrite(uint8_t n,uint8_t state){
  return relayState(n,state);
}

void STM32IO::relayWriteDelay(uint8_t n,uint8_t state,uint32_t delay_ms){
  relayStateDelay(n,state,delay_ms);
}

void STM32IO::setTime(time_t tt){
  sendCommand(EXP_CMD_SET_TIME,(uint8_t *)&tt,4); 
}        
time_t STM32IO::getTime(){
  uint32_t ret;
  if(sendCommand(EXP_CMD_GET_TIME,&ret)){
    return (time_t)ret;
  }
}

void STM32IO::lcd_backlight(uint8_t level){
  sendCommand(EXP_CMD_LCD_BACKLIGHT,&level,1);
}

uint8_t STM32IO::lcd_backlight(){
  uint32_t ret;  
  if(sendCommand(EXP_CMD_LCD_BACKLIGHT,&ret)){
    return ret&0xFF;
  }
  return 0;
}

void STM32IO::lcd_command(uint8_t cmd){
  sendCommand(EXP_CMD_LCD_COMMAND,&cmd,1);
}
void STM32IO::lcd_write(const uint8_t * buf,uint8_t len){
  sendCommand(EXP_CMD_LCD_WRITE,buf,len);    
}
void STM32IO::lcd_write(uint8_t buf){
  sendCommand(EXP_CMD_LCD_WRITE,&buf,1);
}
void STM32IO::lcd_setCursor(uint8_t col,uint8_t row){
  uint8_t buf[2] = {col,row};
  sendCommand(EXP_CMD_LCD_SETCURSOR,buf,2);
}
void STM32IO::lcd_clear(){sendCommand(EXP_CMD_LCD_CLEAR);}
void STM32IO::lcd_home(){sendCommand(EXP_CMD_LCD_HOME);}
void STM32IO::lcd_noDisplay(){sendCommand(EXP_CMD_LCD_NODISPLAY);}
void STM32IO::lcd_display(){sendCommand(EXP_CMD_LCD_DISPLAY);}
void STM32IO::lcd_noBlink(){sendCommand(EXP_CMD_LCD_NOBLINK);}
void STM32IO::lcd_blink(){sendCommand(EXP_CMD_LCD_BLINK);}
void STM32IO::lcd_noCursor(){sendCommand(EXP_CMD_LCD_NOCURSOR);}
void STM32IO::lcd_cursor(){sendCommand(EXP_CMD_LCD_CURSOR);}
void STM32IO::lcd_scrollDisplayLeft(){sendCommand(EXP_CMD_LCD_SCRL_LEFT);}
void STM32IO::lcd_scrollDisplayRight(){sendCommand(EXP_CMD_LCD_SCRL_RIGHT);}
void STM32IO::lcd_leftToRight(){sendCommand(EXP_CMD_LCD_LTR);}
void STM32IO::lcd_rightToLeft(){sendCommand(EXP_CMD_LCD_RTL);}
void STM32IO::lcd_autoscroll(){sendCommand(EXP_CMD_LCD_AUTO_SCRL);}
void STM32IO::lcd_noAutoscroll(){sendCommand(EXP_CMD_LCD_NO_AUTO_SCRL);}
void STM32IO::lcd_createChar(uint8_t position, uint8_t data[]){
  uint8_t buf[9];
  buf[0]=position;
  memcpy(&buf[1],data,8);
  sendCommand(EXP_CMD_LCD_CREATE_CHAR,buf,9);
}

uint8_t STM32IO::eepromRead(uint16_t addr){
  uint32_t u32 = eepromRead32(addr);
  return ((uint8_t*)&u32)[0];
}
uint32_t STM32IO::eepromRead32(uint16_t addr){
  uint32_t ret;
  sendCommand(EXP_CMD_EEPROM_READ,(uint8_t*)&addr,4,&ret);
  uint8_t*ptr = (uint8_t*)&ret;
  Serial.printf("Result 4 bytes : [%02X, %02X, %02X, %02X]\n",ptr[0],ptr[1],ptr[2],ptr[3]);
  return ret;
}
uint8_t STM32IO::eepromRead(uint16_t addr,uint8_t *buffer,uint8_t len)
{ 
  if(len<=0||len>16)return 0;  
  uint8_t w = ((((len-1)>>2)+1));
  uint8_t buf[w*4];
  uint32_t*pbuf = (uint32_t*)buf;
  for(int i=0;i<w;i++){
    pbuf[i] = eepromRead32(addr+(i*4));
  }
  memcpy(buffer,buf,len);
  return w*4;
}
uint16_t STM32IO::eepromRead16(uint16_t addr){
  uint32_t u32 = eepromRead32(addr);
  return ((uint16_t*)&u32)[0];
}
float STM32IO::eepromReadFloat(uint16_t addr){
  uint32_t u32 = eepromRead32(addr);
  return *((float*)&u32);
}
void STM32IO::eepromWrite(uint16_t addr,uint8_t data)
{
  eepromWrite(addr,&data,1);
    
}
void STM32IO::eepromWrite(uint16_t addr,const uint8_t * data, uint8_t len)
{
    uint8_t dlen = ((((len-1)>>1)+1))*2;
    uint8_t buf[dlen+2];
    *((uint16_t*)(&buf[0])) = addr;
    memcpy(&buf[2],data,len);
    sendCommand(EXP_CMD_EEPROM_WRITE,buf,dlen+2);
}
void STM32IO::eepromWrite32(uint16_t addr,uint32_t data){
  eepromWrite(addr,(uint8_t*)&data,4);
}
void STM32IO::eepromWrite16(uint16_t addr,uint16_t data){
  eepromWrite(addr,(uint8_t*)&data,2);
}
void STM32IO::eepromWriteFloat(uint16_t addr,float data){
  eepromWrite(addr,(uint8_t*)&data,4);
}
uint32_t temp_input_state;
uint32_t last_read_input;

void STM32IO::readInput(){
  uint32_t us = micros();
  if(us-last_read_input<10000)return;
  uint32_t ret;
  if(sendCommand(EXP_CMD_INPUT_STATE,&ret)){
    if(buttonStateChangeCallback!=NULL){
      for(int i=0;i<8;i++){
        if(((temp_input_state>>i)&1)!=((ret>>i)&1)) 
        {
          uint8_t n = i+1;
          uint8_t s = ((ret>>i)&1);
          buttonStateChangeCallback(n,s);          
        }
      }
    }
    if(switchStateChangeCallback!=NULL){
      for(int i=8;i<12;i++){
        if(((temp_input_state>>i)&1)!=((ret>>i)&1)) 
        {
          uint8_t n = i-7;
          uint8_t s = ((ret>>i)&1);
          switchStateChangeCallback(n,s);          
        }
      }
    }
    if(relayStateChangeCallback!=NULL){
      for(int i=12;i<16;i++){
        if(((temp_input_state>>i)&1)!=((ret>>i)&1)) 
        {
          uint8_t n = i-11;
          uint8_t s = ((ret>>i)&1);
          relayStateChangeCallback(n,s);          
        }
      }
    }
    temp_input_state = ret;
  }  
  last_read_input = us;
}

void STM32IO::loop(){
  readInput();
}

uint8_t STM32IO::buttonState(uint8_t n){
  if(n<1||n>8)return 0;
  readInput();  
  return (temp_input_state>>(n-1))&1;
}
uint8_t STM32IO::swtichState(uint8_t n){
  if(n<1||n>4)return 0;
  readInput();  
  return (temp_input_state>>(n+7))&1;
}

void STM32IO::switchMode(uint8_t n,RelaySwitchModes mode){
  uint8_t buf[2]={n,mode};
  sendCommand(EXP_CMD_SW_MODE,buf,2);
}

RelaySwitchModes STM32IO::switchMode(uint8_t n){
  uint32_t ret;
  if(sendCommand(EXP_CMD_SW_MODE,&n,0,&ret)){
      return (RelaySwitchModes)ret;
  }
  return SW_MODE_UNDEFINED;
}

void STM32IO::onButtonStateChanged(void (*callback)(uint8_t,uint8_t)){
  buttonStateChangeCallback = callback;
}  
void STM32IO::onSwitchStateChanged(void (*callback)(uint8_t,uint8_t)){
  switchStateChangeCallback = callback;
}
void STM32IO::onRelayStateChanged(void (*callback)(uint8_t,uint8_t)){
  relayStateChangeCallback = callback;
}
uint8_t getCRC8(const uint8_t *data,int length) 
{
   uint8_t crc = 0x00;
   uint8_t extract;
   uint8_t sum;
   for(int i=0;i<length;i++)
   {
      extract = *data;
      for (uint8_t tempI = 8; tempI; tempI--) 
      {
         sum = (crc ^ extract) & 0x01;
         crc >>= 1;
         if (sum)
            crc ^= 0x8C;
         extract >>= 1;
      }
      data++;
   }
   return crc;
}

time_t now(){
    return (time_t)stm.getTime(); 
}