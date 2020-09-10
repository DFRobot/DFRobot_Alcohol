#include "DFRobot_Alcohol.h"

DFRobot_Alcohol::DFRobot_Alcohol()
{
}

DFRobot_Alcohol::~DFRobot_Alcohol()
{
}

/* Write data to the i2c register  */
void DFRobot_Alcohol::WriteAlcohol(uint8_t Reg , uint8_t pdata)
{
  if(Uart_I2C_Flag == I2C_FLAG)
  {
    sendBuf[0] = pdata;
    writeData(Reg ,sendBuf ,1);
  }
  else
  {
    if(pdata == MEASURE_MODE_AUTOMATIC)
    {
      writeData(Reg ,CmdALOHA ,9);
    }
    else if(pdata == MEASURE_MODE_PASSIVE)
    {
      writeData(Reg ,CmdPolling ,9);
    }
  }
}

/* Write data to the i2c register  */
void DFRobot_Alcohol::WriteAlcohol_R(uint8_t Reg , uint8_t pdata)
{
  if(Uart_I2C_Flag == I2C_FLAG)
  {
    sendBuf[0] = pdata;
    writeData(Reg ,sendBuf ,1);
  }
}

/* i2c Read Alcohol concentration data */
int16_t DFRobot_Alcohol::ReadAlcohol(uint8_t Reg)
{
  if(Uart_I2C_Flag == I2C_FLAG)
  {
    readData(Reg ,readBuf ,2);
    return ((int16_t)readBuf[0] << 8) + readBuf[1];
  }
  else
  {
    if(Reg == AUTO_DATA_HIGE_REGISTER){
      readData(0 ,readBuf ,DATA_LEN);
      if(readBuf[0] == 0xFF && readBuf[1] == 0x17)
      {
        return ((int16_t)readBuf[4] << 8) + readBuf[5];
      }
      else
      {
        return -1;
      }
    }else if(Reg == PASS_DATA_HIGE_REGISTER){
      writeData(0 ,CmdRead ,DATA_LEN);
      readData (0 ,readBuf ,DATA_LEN);
      if(readBuf[0] == 0xFF && readBuf[1] == 0x86)
      {
        return ((int16_t)readBuf[6] << 8) + readBuf[7];
      }
      else
      {
        return -1;
      }
    }
  }
}

/* Set active and passive mode */
void DFRobot_Alcohol::SetModes(uint8_t Mode)
{
  if(Mode == MEASURE_MODE_AUTOMATIC) {                            // configure to active mode
    WriteAlcohol(MODE_REGISTER , MEASURE_MODE_AUTOMATIC);
    delay(100);   _M_Flag = 0;                                    // Active mode flag bit
  }else if (Mode == MEASURE_MODE_PASSIVE){                        // Configure to passive mode
    WriteAlcohol(MODE_REGISTER , MEASURE_MODE_PASSIVE); 
    delay(100);   _M_Flag = 1;                                    // Passive mode flag bit
  }else {
    return;
  }
}

/* Reading Alcohol concentration */
float DFRobot_Alcohol::ReadAlcoholData(uint8_t CollectNum)
{
  static uint8_t i = 0 ,j = 0;
  if (CollectNum > 0) {
    for(j = CollectNum - 1;  j > 0; j--) {  AlcoholData[j] = AlcoholData[j-1]; }
    if(_M_Flag == 0) {        
      WriteAlcohol_R(SET_PASSIVE_REGISTER , AUTO_READ_DATA);      delay(100);    // read active data in active mode
      AlcoholData[0] = ReadAlcohol(AUTO_DATA_HIGE_REGISTER);
      if(AlcoholData[0] == -1) 
      {
        memset(AlcoholData,0,CollectNum);
        i = 0;
        j = 0;
        return -1;
      }
    }else if(_M_Flag == 1) {
      WriteAlcohol_R(SET_PASSIVE_REGISTER , PASSIVE_READ_DATA);   delay(100);    // read passive data in passive mode, first request once, then read the data
      AlcoholData[0] = ReadAlcohol(PASS_DATA_HIGE_REGISTER);
      if(AlcoholData[0] == -1) 
      {
        memset(AlcoholData,0,CollectNum);
        i = 0;
        j = 0;
        return -1;
      }
    }
    if(i < CollectNum) i++;
    return getAverageNum(AlcoholData, i);
  }else if(CollectNum <= 0 || CollectNum > 100){
   return -1;
  }
}

/* Get the average data */
float DFRobot_Alcohol::getAverageNum(int bArray[], int iFilterLen) 
{
  uint8_t i = 0;
  unsigned long bTemp = 0;
  for(i = 0; i < iFilterLen; i++)
  {
    bTemp += bArray[i];
  }
  return (float)(bTemp / iFilterLen) / 1000.0;
}

DFRobot_Alcohol_I2C::DFRobot_Alcohol_I2C(TwoWire *pWire, uint8_t addr)
{
  _pWire = pWire;
  this->_I2C_addr = addr;
  Uart_I2C_Flag = I2C_FLAG;
}

bool DFRobot_Alcohol_I2C::begin()
{
  _pWire->begin();
  _pWire->beginTransmission(_I2C_addr);
  if(_pWire->endTransmission() == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void DFRobot_Alcohol_I2C::writeData(uint8_t Reg ,uint8_t *Data ,uint8_t len)
{
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(Reg);
  for(uint8_t i = 0; i < len; i++)
  {
    _pWire->write(Data[i]);
  }
  _pWire->endTransmission();
}

uint8_t DFRobot_Alcohol_I2C::readData(uint8_t Reg,uint8_t *Data,uint8_t len)
{
  int i=0;
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(Reg);
  _pWire->endTransmission();
  _pWire->requestFrom((uint8_t)this->_I2C_addr,(uint8_t)len);
  while (_pWire->available())
  {
    Data[i++]=_pWire->read();
  }
}

#ifdef ESP_PLATFORM
  DFRobot_Alcohol_UART::DFRobot_Alcohol_UART(HardwareSerial *sSerial ,uint16_t Baud)
  {
    this->_serial = sSerial;
    this->_Baud = Baud;
    Uart_I2C_Flag = UART_FLAG;
  }
#else
  DFRobot_Alcohol_UART::DFRobot_Alcohol_UART(SoftwareSerial *sSerial ,uint16_t Baud)
  {
    this->_serial = sSerial;
    this->_Baud = Baud;
    Uart_I2C_Flag = UART_FLAG;
  }
#endif

bool DFRobot_Alcohol_UART::begin()
{
  uint8_t i = 0;
  _serial->begin(this->_Baud);
  this->writeData(0,CmdALOHA,DATA_LEN);
  this->readData (0,readBuf,DATA_LEN);
  if(readBuf[0] == 0xFF && readBuf[1] == 0x17)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void DFRobot_Alcohol_UART::writeData(uint8_t Reg ,uint8_t *Data ,uint8_t len)
{
  uint8_t i = 0;
  for(i = 0; i < len; i++)
  {
    _serial->print((char)Data[i]);
  }
}

uint8_t DFRobot_Alcohol_UART::readData(uint8_t Reg,uint8_t *Data,uint8_t len)
{
  uint8_t i = 0;
  memset(readBuf,0,DATA_LEN);
  uint32_t nowtime = millis();
  while(millis() - nowtime < TIME_OUT)
  {
    while(_serial->available() > 0)
    {
      readBuf[i++] = _serial->read();
    }
  }
}