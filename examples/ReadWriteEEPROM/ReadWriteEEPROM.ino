#include <STM32IOLib.h>
STM32IO stm;
void setup(){
  Serial.begin(115200);
  Serial.print("\nInitializing...");
  stm.begin(true);
  delay(100);  
  Serial.println("Done.");
  delay(1000);
  float random = stm.analogRead(0)*1000;//Random Data from analog pin A0 => connect A0 to SW1 terminal
  uint32_t t = *((uint32_t*)&random);//Convert float to UInt32
  uint16_t base_address = t%100;//Random Address
  uint8_t data_8 = t&0xFF;
  uint8_t data_ar[4] = {data_8,data_8+1,data_8+2,data_8+3};
  uint16_t data_16 = t&0xFFFF;
  uint32_t data_32 = t;
  float data_float = random;
  
  uint16_t address = base_address;
  Serial.printf("Write 1 byte [%u] to address [%08u]\n",data_8,address);
  stm.eepromWrite(address,data_8);
  address+=1;

  Serial.printf("Write 4 bytes [%u,%u,%u,%u] to address [%08u]\n",data_ar[0],data_ar[1],data_ar[2],data_ar[3],address);
  stm.eepromWrite(address,data_ar,4);
  address+=4;

  Serial.printf("Write UInt16 [%04X] to address [%08u]\n",data_16,address);
  stm.eepromWrite16(address,data_16);
  address+=2;

  Serial.printf("Write UInt32 [%08X] to address [%08u]\n",data_32,address);
  stm.eepromWrite32(address,data_32);
  address+=4;

  Serial.printf("Write float [%.2f] to address [%08u]\n",data_float,address);
  stm.eepromWriteFloat(address,data_float);
  
  address = base_address;
  
  uint8_t data_out_8 = stm.eepromRead(address);
  Serial.printf("Read 1 byte from address [%08u] : [%u] ==> %s\n",address,data_out_8,data_out_8==data_8?"Pass":"Fail");
  address+=1;

  uint8_t data_out_ar[4] = {0,0,0,0};
  stm.eepromRead(address,data_out_ar,4);
  Serial.printf("Read 4 byte from address [%08u] : [%u,%u,%u,%u] ==>%s\n"
    ,address
    ,data_out_ar[0],data_out_ar[1],data_out_ar[2],data_out_ar[3]
    ,data_out_ar[0] == data_ar[0] && data_out_ar[1] == data_ar[1] && data_out_ar[2] == data_ar[2] && data_out_ar[3] == data_ar[3]?"Pass":"Fail");
  address+=4;

  uint16_t data_out_16 = stm.eepromRead16(address);
  Serial.printf("Read UInt16 from address [%08u] : [%04X] ==> %s\n",address,data_out_16,data_out_16==data_16?"Pass":"Fail");
  address+=2;

  uint32_t data_out_32 = stm.eepromRead32(address);
  Serial.printf("Read UInt32 from address [%08u] : [%08X] ==> %s\n",address,data_out_32,data_out_32==data_32?"Pass":"Fail");
  address+=4;

  float data_out_float = stm.eepromReadFloat(address);
  Serial.printf("Read float from address [%08u] : [%.2f] ==> %s\n",address,data_out_float, data_out_float==data_float?"Pass":"Fail");
  address+=1;
}
void loop() {
  
}