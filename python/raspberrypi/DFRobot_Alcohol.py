# -*- coding: utf-8 -*
'''!
  @file DFRobot_Alcohol.py
  @brief DFRobot_Alcohol Class infrastructure, implementation of underlying methods
  @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license The MIT License (MIT)
  @author [ZhixinLiu](zhixin.liu@dfrobot.com)
  @version V1.0
  date 2020-09-09
  @url https://github.com/DFRobot/DFRobot_Alcohol
'''
import serial
import time
import smbus

ALCOHOL_ADDRESS_0         = 0x72
ALCOHOL_ADDRESS_1         = 0x73
ALCOHOL_ADDRESS_2         = 0x74
ALCOHOL_ADDRESS_3         = 0x75

## active  mode
MEASURE_MODE_AUTOMATIC    = 0x00
## passive mode
MEASURE_MODE_PASSIVE      = 0x01
## auto read alcohol data
AUTO_READ_DATA            = 0x00
## passive read alcohol data
PASSIVE_READ_DATA         = 0x01
## mode register
MODE_REGISTER             = 0x03
## read alcohol data register
SET_PASSIVE_REGISTER      = 0x04
## auto data high eight bits
AUTO_DATA_HIGE_REGISTER   = 0x09
## auto data Low  eight bits
AUTO_DATA_LOW_REGISTER    = 0x0A
## auto data high eight bits
PASS_DATA_HIGE_REGISTER   = 0x0B
## auto data Low  eight bits
PASS_DATA_LOW_REGISTER    = 0x0C
I2C_MODE                  = 0x01
UART_MODE                 = 0x02
ERROR                     = -1
  
class DFRobot_Alcohol(object):
  __m_flag   = 0                # mode flag
  __count    = 0                # acquisition count    
  __txbuf        = [0]          # i2c send buffer
  __alcoholdata  = [0]*101      # alcohol data
  __uart_i2c     =  0
  def __init__(self, bus, Baud):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self.__uart_i2c = I2C_MODE;
    else:
      self.ser = serial.Serial("/dev/ttyAMA0", baudrate=Baud,stopbits=1, timeout=0.5)
      self.__uart_i2c = UART_MODE;
      if self.ser.isOpen == False:
        self.ser.open()

  def set_mode(self, mode):
    '''!
      @brief set the mode
      @param MEASURE_MODE_AUTOMATIC  active mode
      @param MEASURE_MODE_PASSIVE    passive mode
    '''
    if mode ==  MEASURE_MODE_AUTOMATIC:
      self.__m_flag = 0
      self.__txbuf[0] = MEASURE_MODE_AUTOMATIC 
      self.write_reg(MODE_REGISTER, self.__txbuf)
    elif mode == MEASURE_MODE_PASSIVE:
      self.__m_flag = 1
      self.__txbuf[0] = MEASURE_MODE_PASSIVE
      self.write_reg(MODE_REGISTER, self.__txbuf)
    else:
      self.__m_flag = 2
      return

  def get_alcohol_data(self, collectnum):
    '''!
      @brief get the alcohol data, units of PPM
      @param collectnum Collect the number
      @return  alcohol concentration, (units PPM)
    '''
    if collectnum > 0:
      for num in range(collectnum, 1, -1):
        self.__alcoholdata[num-1] = self.__alcoholdata[num-2];
      if self.__m_flag == 0:
        self.__txbuf[0] = AUTO_READ_DATA
        self.write_reg(SET_PASSIVE_REGISTER, self.__txbuf)
        self.__alcoholdata[0] = self.get_alcohol(AUTO_DATA_HIGE_REGISTER)
        if self.__alcoholdata[0] == -1:
          for num in range (0, collectnum):
            self.__alcoholdata[num] = 0
          return -1
      elif self.__m_flag == 1:
        self.__txbuf[0] = PASSIVE_READ_DATA
        self.write_reg(SET_PASSIVE_REGISTER, self.__txbuf)
        self.__alcoholdata[0] = self.get_alcohol(PASS_DATA_HIGE_REGISTER)
        if self.__alcoholdata[0] == -1:
          for num in range (0, collectnum):
            self.__alcoholdata[num] = 0
          return -1
      if self.__count < collectnum:
        self.__count += 1
      return self.get_average_num(self.__alcoholdata, self.__count)
    elif (collectnum > 100) or (collectnum <= 0):
      return -1

  def get_average_num(self, barry, Len):
    '''!
      @brief get the average of the alcohol data, units of PPM
      @param barry alcohol data group
      @param Len The number of data
      @return  alcohol concentration, (units PPM)
    '''
    temp = 0
    for num in range (0, Len):
      temp += barry[num]
    return (temp / Len) / 1000.0

  def get_alcohol(self,reg):
    '''!
      @brief get the alcohol data, units of PPM
      @param reg register address
      @return  alcohol concentration, (units PPM)
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(reg, 2)
      return ((rslt[0] << 8) + rslt[1])
    elif self.__uart_i2c == UART_MODE:
      rslt = self.read_reg(reg, 9)
      if rslt[0] == 0xff and rslt[1] == 0x17:
        number = rslt[4]<<8
        number += rslt[5]
        return number
      elif rslt[0] == 0xff and rslt[1] == 0x86:
        number = rslt[6]<<8
        number += rslt[7]
        return number
      else:
        return -1

class DFRobot_Alcohol_I2C(DFRobot_Alcohol): 
  def __init__(self, bus, addr):
    self.__addr = addr;
    super(DFRobot_Alcohol_I2C, self).__init__(bus,0)

  def write_reg(self, reg, data):
    while 1:
      try:
        self.i2cbus.write_i2c_block_data(self.__addr, reg, data)
        return
      except:
        print("please check connect!")
        time.sleep(1)

  def read_reg(self, reg, len):
    try:
      rslt = self.i2cbus.read_i2c_block_data(self.__addr, reg, len)
    except:
      rslt = -1
    return rslt

class DFRobot_Alcohol_UART(DFRobot_Alcohol): 
  __CmdALOHA   = [0xFF,0x01,0x78,0x40,0x00,0x00,0x00,0x00,0x47]
  __CmdPolling = [0xFF,0x01,0x78,0x41,0x00,0x00,0x00,0x00,0x46]
  __CmdRead    = [0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79]


  def __init__(self, Baud):
    self.__Baud = Baud;
    super(DFRobot_Alcohol_UART, self).__init__(0,Baud)

  def write_reg(self, reg, data):
    if data[0] == MEASURE_MODE_AUTOMATIC and reg == MODE_REGISTER:
      self.ser.write(self.__CmdALOHA)
    elif data[0] == MEASURE_MODE_PASSIVE and reg == MODE_REGISTER:
      self.ser.write(self.__CmdPolling)
    elif data[0] == PASSIVE_READ_DATA    and reg == SET_PASSIVE_REGISTER:
      self.ser.write(self.__CmdRead)
    else:
      return;

  def read_reg(self, reg, len):
    timenow = time.time()
    recv = [0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
    i = 0
    while(time.time() - timenow) <= 1:
      count = self.ser.inWaiting()
      if count != 0:
        recv = self.ser.read(count)
        self.ser.flushInput()
        recv =[ord(c) for c in recv]
    return recv