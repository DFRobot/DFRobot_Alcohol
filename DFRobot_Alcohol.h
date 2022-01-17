/*!
 * @file DFRobot_Sensor.h
 * @brief Define the basic structure of the DFRobot_Alcohol class, the implementation of the basic methods
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2020-09-09
 * @url https://github.com/DFRobot/DFRobot_Sensor
 */
#ifndef __DFROBOT_ALCOHOL_H__
#define __DFROBOT_ALCOHOL_H__

#include <Arduino.h>
#include <Wire.h>
#ifdef ESP_PLATFORM
  #include <HardwareSerial.h>
#else
  #include <SoftwareSerial.h>
#endif

#define           ALCOHOL_ADDRESS_0         0x72
#define           ALCOHOL_ADDRESS_1         0x73
#define           ALCOHOL_ADDRESS_2         0x74
#define           ALCOHOL_ADDRESS_3         0x75
#define           MEASURE_MODE_AUTOMATIC    0x00           ///< active  mode
#define           MEASURE_MODE_PASSIVE      0x01           ///< passive mode
#define           AUTO_READ_DATA            0x00           ///< auto read ozone data
#define           PASSIVE_READ_DATA         0x01           ///< passive read ozone data
#define           MODE_REGISTER             0x03           ///< mode register
#define           SET_PASSIVE_REGISTER      0x04           ///< read ozone data register
#define           AUTO_DATA_HIGE_REGISTER   0x09           ///< AUTO data high eight bits
#define           AUTO_DATA_LOW_REGISTER    0x0A           ///< AUTO data Low  eight bits
#define           PASS_DATA_HIGE_REGISTER   0x0B           ///< AUTO data high eight bits
#define           PASS_DATA_LOW_REGISTER    0x0C           ///< AUTO data Low  eight bits
#define           OCOUNT                    100            ///< Ozone Count Value
#define           DATA_LEN                  9              ///< Uart data len
#define           I2C_FLAG                  1
#define           UART_FLAG                 2
#define           TIME_OUT                  500            ///< time out
#define           ERROR                     -1

class DFRobot_Alcohol{
public:
  DFRobot_Alcohol();
  ~DFRobot_Alcohol();
  /**
   * @fn setModes
   * @brief Set mode Active or passive measurement.
   * @param mode mode is Active or passive.
   * @n       MEASURE_MODE_AUTOMATIC    active  mode
   * @n       MEASURE_MODE_PASSIVE      passive mode
   * @return None
   */
  void setModes(uint8_t mode);

  /**
   * @fn readAlcoholData
   * @brief read Alcohol data.
   * @param collectNum is the number of data collected,(The default value is 20)
   * @n       COLLECT_NUMBER       The collection range is 1-100
   * @return Alcohol concentration: one part per million (PPM).
   */
  float readAlcoholData(uint8_t collectNum = 20);
  uint8_t  readBuf[14]   = {0};
  uint8_t  sendBuf[14]   = {0};
  uint8_t  cmdALOHA[9]   = {0xFF,0x01,0x78,0x40,0x00,0x00,0x00,0x00,0x47};
  uint8_t  cmdPolling[9] = {0xFF,0x01,0x78,0x41,0x00,0x00,0x00,0x00,0x46};
  uint8_t  cmdRead[9]    = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
  uint8_t  uartI2CFlag = 0;
private:
  void     writeAlcohol(uint8_t reg,  uint8_t pdata);
  void     writeAlcohol_R(uint8_t reg,  uint8_t pdata);
  int16_t  readAlcohol(uint8_t reg);
  int      alcoholData[OCOUNT] = {0x00};
  float    getAverageNum(int bArray[], int iFilterLen);
  uint8_t  _addr;
  uint8_t  _M_Flag = 0;

  virtual void writeData(uint8_t reg, uint8_t *data, uint8_t len) = 0;
  virtual int16_t readData(uint8_t reg, uint8_t *data, uint8_t len) = 0;
};

class DFRobot_Alcohol_I2C:public DFRobot_Alcohol{
public:
  DFRobot_Alcohol_I2C(TwoWire *pWire=&Wire, uint8_t addr = 0x75);
  bool begin(void);
protected:
  virtual void writeData(uint8_t reg, uint8_t *data, uint8_t len);
  virtual int16_t readData(uint8_t reg, uint8_t *data, uint8_t len);
private:
  TwoWire *_pWire;
  uint8_t _I2C_addr;
};

class DFRobot_Alcohol_UART:public DFRobot_Alcohol{
public:
#ifdef ESP_PLATFORM
  DFRobot_Alcohol_UART(HardwareSerial *hSerial, uint16_t Baud);
#else
  DFRobot_Alcohol_UART(SoftwareSerial *sSerial, uint16_t Baud);
#endif

  bool begin(void);
protected:
  virtual void writeData(uint8_t reg, uint8_t *data, uint8_t len);
  virtual int16_t readData(uint8_t reg, uint8_t *data, uint8_t len);
private:
#ifdef ESP_PLATFORM
  HardwareSerial *_serial;
#else
  SoftwareSerial *_serial;
#endif
  uint32_t _baud;
};
#endif