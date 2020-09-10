# DFRobot_AlcoholSensor
DFRobot's AlcoholSensor

## DFRobot_AlcoholSensor Library for Arduino
---------------------------------------------------------
Arduino library is provided for reading alcohol concentrations through I2C or uart.
Used to read the alcohol concentrations of the current module


## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

<snippet>
<content>

## Installation

To use this library download the zip file, uncompress it to a folder named DFRobot_AlcoholSensor.
Download the zip file first to use this library and uncompress it to a folder named DFRobot_AlcoholSensor.

## Methods

```C++
 /*
  * @brief initialization parameters for i2c.
  * @param Wire is I2C Class
  * @param Alcohol_I2C_Address is i2c address
  */
   DFRobot_Alcohol_I2C  Alcohol(&Wire, Alcohol_I2C_Address);

 /* @brief Initializes the soft serial port
  * @param RX is uart receive io
  * @param TX is uart tranfer io
  */
   mySerial(RX, TX);

 /*
  * @brief initialization parameters for arduino software uart.
  * @param sSerial is uart Class
  * @param Baud is uart baud
  */
   DFRobot_Alcohol_UART Alcohol(SoftwareSerial *sSerial ,uint16_t Baud)

 /*
  * @brief initialization parameters for ESP32 hardware uart.
  * @param sSerial is uart Class
  * @param Baud is uart baud
  */
   DFRobot_Alcohol_UART(HardwareSerial *sSerial ,uint16_t Baud);

 /*
  * @brief initialization parameters for i2c or uart.
  * @return true and false.
  */
   bool begin();

 /*
  * @brief Set mode Active or passive measurement.
  *
  * @param Mode is Active or passive.
  *    MEASURE_MODE_AUTOMATIC                 // active  mode
  *    MEASURE_MODE_PASSIVE                   // passive mode
  */
   void SetModes(uint8_t Mode);

 /*
  * @brief read Alcohol data.
  *
  * @param CollectNum is the number of data collected,(The default value is 20)
  *    COLLECT_NUMBER                         // The collection range is 1-100
  *
  * @return Alcohol concentration: one part per million (PPM).
  */
   float ReadAlcoholData(uint8_t CollectNum = 20);

```
## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
FireBeetle-ESP32  |      √       |             |            | 
ATMEGA2560  |      √       |             |            | 
Arduino uno |       √      |             |            | 
Leonardo  |      √       |              |             | 

## History

- September 9, 2020 - Version 1.0 released.


## Credits

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2020. (Welcome to our [website](https://www.dfrobot.com/))