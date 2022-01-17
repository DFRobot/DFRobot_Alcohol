# DFRobot_Alcohol
- [中文版](./README_CN.md)

This is an Arduino-compatible alcohol concentration sensor module, with a measurement range of 0-5ppm, and supports both I2C and UART output methods. After factory calibration, it can quickly and accurately measure the concentration of alcohol vapor in the environment. It is suitable for alcohol vapor concentration detection in drunk driving detection, automobile intelligence and other low-concentration scenarios. 
![效果图](resources/images/sen0376.jpg)


## Product Link（https://www.dfrobot.com/product-2186.html）

    SKU：SEN0376

## Table of Contents

* [Summary](#Summary)
* [Installation](#Installation)
* [Methods](#Methods)
* [Compatibility](#Compatibility)
* [History](#History)
* [Credits](#Credits)

## Summary

Using the electrochemical principle, it has been calibrated before leaving the factory, which can accurately measure the alcohol concentration in the environment. And it has the characteristics of strong anti-interference ability, high stability and high sensitivity, and has a service life of up to two years. Resolution can reach 0.01ppm (10ppb), support 3.3~5.5V wide voltage input, with two data output types: I2C and UART。

## Installation
There are two methods for using this library:<br>
1. Open Arduino IDE, search for "DFRobot_Alcohol" on the status bar in Tools ->Manager Libraries and install the library.<br>
2. Download the library file before use, paste it into \Arduino\libraries directory, then open the examples folder and run the demo in the folder.<br>

## Methods

```C++
 /**
  * @fn mySerial
  * @brief Initializes the soft serial port
  * @param RX is uart receive io
  * @param TX is uart tranfer io
  * @return None
  */
   mySerial(RX, TX);

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

```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | :----:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
Micro:bit          |              |              |      √      |


## History

- 2020/9/9 - V1.0.0 Version
## Credits

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2020. (Welcome to our [website](https://www.dfrobot.com/))
