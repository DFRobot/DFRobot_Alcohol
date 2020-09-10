# -*- coding:utf-8 -*-
""" 
  @file get_alcohol_data.py
  @brief Reading alcohol concentration, A concentration of one part per million (PPM).
  @n step: we must first determine the i2c device address, will dial the code switch A0, A1 (ADDRESS_0 for [0 0]), (ADDRESS_1 for [1 0]), (ADDRESS_2 for [0 1]), (ADDRESS_3 for [1 1]).
  @n       Then configure the mode of active and passive acquisition, Finally, alcohol data can be read.
  @n note: it takes time to stable alcohol concentration, about 1 minutes.
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @licence     The MIT License (MIT)
  @author      [ZhixinLiu](zhixin.liu@dfrobot.com)
  version  V1.0
  date  2020-09-09
  @get from https://www.dfrobot.com
  @url https://github.com/DFRobot/DFRobot_Alcohol
"""
import sys
sys.path.append("../..")
from DFRobot_Alcohol import *

COLLECT_NUMBER   = 1               # collect number, the collection range is 1-100
I2C_MODE         = 0x01            # default use I2C1
'''
   # The first  parameter is to select i2c0 or i2c1
   # The second parameter is the i2c device address
   # The default address for i2c is ADDRESS_3
   # ADDRESS_0                 = 0x72
   # ADDRESS_1                 = 0x73
   # ADDRESS_2                 = 0x74
   # ADDRESS_3                 = 0x75
'''
alcohol = DFRobot_Alcohol_I2C (I2C_MODE ,ADDRESS_3)
#alcohol = DFRobot_Alcohol_UART(9600)
'''
   # The module is configured in automatic mode or passive
   # MEASURE_MODE_AUTOMATIC    = 0x00           # active  mode
   # MEASURE_MODE_PASSIVE      = 0x01           # passive mode
''' 
alcohol.set_mode(MEASURE_MODE_AUTOMATIC)
time.sleep(1)
while(1):
  ''' Smooth data collection £¬the collection range is 1-100 '''
  alcohol_concentration = alcohol.get_alcohol_data(COLLECT_NUMBER);
  if alcohol_concentration == ERROR:
    print("Please check the connection !")
  else:
    print("alcohol concentration is %.2f PPM."%alcohol_concentration)
  time.sleep(1)