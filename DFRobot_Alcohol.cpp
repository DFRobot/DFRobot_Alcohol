/*!
 * @file DFRobot_Alcohol.cpp
 * @brief Define the basic structure of the DFRobot_Alcohol class, the implementation of the basic methods
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2020-09-09
 * @url https://github.com/DFRobot/DFRobot_Alcohol
 */
#include "DFRobot_Alcohol.h"

DFRobot_Alcohol::DFRobot_Alcohol(){}
DFRobot_Alcohol::~DFRobot_Alcohol(){}

void DFRobot_Alcohol::writeAlcohol(uint8_t reg, uint8_t pdata)
{
  if(uartI2CFlag == I2C_FLAG){
    sendBuf[0] = pdata;
    writeData(reg, sendBuf, 1);
  }else{
    if(pdata == MEASURE_MODE_AUTOMATIC){
      writeData(reg, cmdALOHA, 9);
    }else if(pdata == MEASURE_MODE_PASSIVE){
      writeData(reg, cmdPolling, 9);
    }
  }
}

void DFRobot_Alcohol::writeAlcohol_R(uint8_t reg, uint8_t pdata)
{
  if(uartI2CFlag == I2C_FLAG){
    sendBuf[0] = pdata;
    writeData(reg, sendBuf, 1);
  }
}

int16_t DFRobot_Alcohol::readAlcohol(uint8_t reg)
{
  if(uartI2CFlag == I2C_FLAG){
    if(-1 == readData(reg, readBuf, 2)){
      return -1;
    }else{
      return ((int16_t)readBuf[0] << 8) + readBuf[1];
    }
  }else{
    if(reg == AUTO_DATA_HIGE_REGISTER){
      readData(0, readBuf, DATA_LEN);
      if(readBuf[0] == 0xFF && readBuf[1] == 0x17){
        return ((int16_t)readBuf[4] << 8) + readBuf[5];
      }else{
        return -1;
      }
    }else if(reg == PASS_DATA_HIGE_REGISTER){
      writeData(0, cmdRead, DATA_LEN);
      readData (0, readBuf, DATA_LEN);
      if(readBuf[0] == 0xFF && readBuf[1] == 0x86){
        return ((int16_t)readBuf[6] << 8) + readBuf[7];
      }else{
        return -1;
      }
    }
  }
  return 0;
}

void DFRobot_Alcohol::setModes(uint8_t mode)
{
  if(mode == MEASURE_MODE_AUTOMATIC){
    writeAlcohol(MODE_REGISTER,  MEASURE_MODE_AUTOMATIC);
    delay(100);   _M_Flag = 0;
  }else if (mode == MEASURE_MODE_PASSIVE){
    writeAlcohol(MODE_REGISTER,  MEASURE_MODE_PASSIVE);
    delay(100);   _M_Flag = 1;
  }else {
    return;
  }
}

float DFRobot_Alcohol::readAlcoholData(uint8_t collectNum)
{
  static uint8_t i = 0, j = 0;
  if (collectNum > 0) {
    for(j = collectNum - 1;  j > 0; j--) {  alcoholData[j] = alcoholData[j-1]; }
    if(_M_Flag == 0){
      writeAlcohol_R(SET_PASSIVE_REGISTER,  AUTO_READ_DATA);      delay(100);
      alcoholData[0] = readAlcohol(AUTO_DATA_HIGE_REGISTER);
      if(alcoholData[0] == -1) {
        memset(alcoholData,0,collectNum);
        i = 0;
        j = 0;
        return -1;
      }
    }else if(_M_Flag == 1){
      writeAlcohol_R(SET_PASSIVE_REGISTER,  PASSIVE_READ_DATA);   delay(100);
      alcoholData[0] = readAlcohol(PASS_DATA_HIGE_REGISTER);
      if(alcoholData[0] == -1) {
        memset(alcoholData,0,collectNum);
        i = 0;
        j = 0;
        return -1;
      }
    }
    if(i < collectNum) i++;
    return getAverageNum(alcoholData, i);
  }else if(collectNum <= 0 || collectNum > 100){
   return -1;
  }
  return 0.0;
}

float DFRobot_Alcohol::getAverageNum(int bArray[], int iFilterLen) 
{
  uint8_t i = 0;
  unsigned long bTemp = 0;
  for(i = 0; i < iFilterLen; i++){
    bTemp += bArray[i];
  }
  return (float)(bTemp / iFilterLen) / 1000.0;
}

DFRobot_Alcohol_I2C::DFRobot_Alcohol_I2C(TwoWire *pWire, uint8_t addr)
{
  _pWire = pWire;
  this->_I2C_addr = addr;
  uartI2CFlag = I2C_FLAG;
}

bool DFRobot_Alcohol_I2C::begin()
{
  _pWire->begin();
  _pWire->beginTransmission(_I2C_addr);
  if(_pWire->endTransmission() == 0){
    return true;
  }else{
    return false;
  }
}

void DFRobot_Alcohol_I2C::writeData(uint8_t reg, uint8_t *data, uint8_t len)
{
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(reg);
  for(uint8_t i = 0; i < len; i++){
    _pWire->write(data[i]);
  }
  _pWire->endTransmission();
}

int16_t DFRobot_Alcohol_I2C::readData(uint8_t reg,uint8_t *data,uint8_t len)
{
  int i=0;
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(reg);
  if(_pWire->endTransmission() != 0){
    return -1;
  }
  _pWire->requestFrom((uint8_t)this->_I2C_addr,(uint8_t)len);
  while (_pWire->available()){
    data[i++]=_pWire->read();
  }
  return 0;
}

#ifdef ESP_PLATFORM
  DFRobot_Alcohol_UART::DFRobot_Alcohol_UART(HardwareSerial *hSerial, uint16_t Baud)
  {
    this->_serial = hSerial;
    this->_baud = Baud;
    uartI2CFlag = UART_FLAG;
  }
#else
  DFRobot_Alcohol_UART::DFRobot_Alcohol_UART(SoftwareSerial *sSerial, uint16_t Baud)
  {
    this->_serial = sSerial;
    this->_baud = Baud;
    uartI2CFlag = UART_FLAG;
  }
#endif

bool DFRobot_Alcohol_UART::begin()
{
  _serial->begin(this->_baud);
  this->writeData(0,cmdALOHA,DATA_LEN);
  this->readData (0,readBuf,DATA_LEN);
  if(readBuf[0] == 0xFF && readBuf[1] == 0x17){
    return true;
  }else{
    return false;
  }
}

void DFRobot_Alcohol_UART::writeData(uint8_t reg, uint8_t *data, uint8_t len)
{
  uint8_t i = 0;
  for(i = 0; i < len; i++){
    _serial->print((char)data[i]);
  }
}

int16_t DFRobot_Alcohol_UART::readData(uint8_t reg, uint8_t *data, uint8_t len)
{
  uint8_t i = 0;
  memset(readBuf,0,DATA_LEN);
  uint32_t nowtime = millis();
  while(millis() - nowtime < TIME_OUT){
    while(_serial->available() > 0){
      readBuf[i++] = _serial->read();
    }
  }
  return 0;
}