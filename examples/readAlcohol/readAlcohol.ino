 /*!
  * @file  readAlcohol.ino
  * @brief Reading Alcohol concentration, A concentration of one part per million (PPM).
  * @n step: we must first determine the iic device address, will dial the code switch A0, A1 (ADDRESS_0 for [0 0]), (ADDRESS_1 for [1 0]), (ADDRESS_2 for [0 1]), (ADDRESS_3 for [1 1]).
  *
  * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @licence     The MIT License (MIT)
  * @author      ZhixinLiu(zhixin.liu@dfrobot.com)
  * @version     V1.0
  * @date        2020-09-09
  * @get         from https://www.dfrobot.com
  * @url         https://github.com/dfrobot/DFRobot_Alcohol
  */

#include "DFRobot_Alcohol.h"

#define COLLECT_NUMBER   5                      // collect number, the collection range is 1-100
#define ALCOHOL_I2C_ADDRESS ADDRESS_3
/*   iic slave Address, The default is ADDRESS_3
       ADDRESS_0               0x72             // i2c device address
       ADDRESS_1               0x73
       ADDRESS_2               0x74
       ADDRESS_3               0x75
*/
//DFRobot_Alcohol_I2C Alcohol(&Wire ,ALCOHOL_I2C_ADDRESS);

#ifdef ESP_PLATFORM
  // ESP32 user hardware uart
  // RX io16
  // TX io17
  DFRobot_Alcohol_UART Alcohol(&Serial2 ,9600);
#else
  // Arduino user software uart
  // RX io10
  // TX io11
  SoftwareSerial              mySerial(10 ,11);
  DFRobot_Alcohol_UART Alcohol(&mySerial ,9600);
#endif

void setup() 
{
  Serial.begin(9600);
  while(!Alcohol.begin())
  {
    Serial.println("NO Deivces !");
    delay(1000);
  }
  Serial.println("Device connected successfully !");

/*  Set iic mode, active mode or passive mode
      MEASURE_MODE_AUTOMATIC            // active  mode
      MEASURE_MODE_PASSIVE              // passive mode
*/
  Alcohol.SetModes(MEASURE_MODE_AUTOMATIC);
}


void loop() 
{
/*  Smooth data collection
      COLLECT_NUMBER                    // The collection range is 1-100
*/
  float alcoholConcentration = Alcohol.ReadAlcoholData(COLLECT_NUMBER);
  if(alcoholConcentration == ERROR)
  {
    Serial.println("Please check the connection !");
  }else{
    Serial.print("Alcohol concentration is ");
    Serial.print(alcoholConcentration);
    Serial.println(" PPM.");
  }
  delay(1000);
}