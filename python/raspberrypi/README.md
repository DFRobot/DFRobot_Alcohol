# DFRobot alcohol concentration sensor

This RaspberryPi alcohol sensor board can communicate with RaspberryPi via I2C or uart.<br>
alcohol sensor can measure data from 0-5 PPM.<br>
alcohol sensor Long service life.<br>
alcohol sensor Excellent stability and anti-interference.<br>
alcohol sensor High sensitivity and low power consumption.<br>
alcohol sensor  Temperature compensation, excellent linear output.<br>

## DFRobot alcohol Library for RaspberryPi

Provide the Raspberry Pi library for the DFRobot_alcohol module.

## Table of Contents

* [Summary](#summary)
* [Feature](#feature)
* [Installation](#installation)
* [Methods](#methods)
* [History](#history)
* [Credits](#credits)

## Summary

alcohol module.

## Feature

1. The module has two modes, one is active data acquisition and the other is passive data acquisition. <br>
2. You can measure the concentration of alcohol in the air. <br>

## Installation

This Sensor should work with DFRobot_alcohol on RaspberryPi. <br>
Run the program:

```
$> python get_alcohol_data.py
```

## Methods

```py

  def set_mode(self ,mode):
    '''
      @brief Set mode active or passive
      @param mode: MEASURE_MODE_AUTOMATIC or MEASURE_MODE_PASSIVE(active or passive)
    '''

  def get_alcohol_data(self ,collectnum):
    '''
      @brief get the alcohol data,units of PPM
      @param collectnum: Collect the number
      @return  alcohol concentration, (PPM)
    '''

```
## History

September 9, 2020 - Version 1.0 released.

## Credits

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2020. (Welcome to our website)