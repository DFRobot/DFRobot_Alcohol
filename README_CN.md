# DFRobot_Alcohol
- [English Version](./README.md)

这是一款Arduino兼容的酒精浓度传感器模组，测量范围0-5ppm，支持I2C和UART两种输出方式，经过出厂标定，可以快速、准确的测量环境中酒精蒸汽的浓度。适用于酒驾检测、汽车智能化及其他小浓度场景下的酒精蒸汽浓度检测。

![正反面svg效果图](resources/images/sen0376.jpg)


## 产品链接（https://www.dfrobot.com.cn/goods-2995.html）

    SKU：SEN0376

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性y)
* [历史](#历史)
* [创作者](#创作者)

## 概述

DFRobot推出的酒精传感器，采用电化学原理，出厂前已经进行过标定，可以准确测量环境中的酒精浓度。<br>并具有抗干扰能力强、稳定性高、灵敏度高等特点，使用寿命长达两年。分辨率可以达到0.01ppm(10ppb)，<br>支持3.3~5.5V宽电压输入，具有I2C和UART两种数据输出类型，兼容Arduino、ESP32、树莓派等主流的主控设备，具有良好的兼容性。简单易用的Gravity接口，配上我们的样例代码，可迅速搭建出自己的酒精浓度检测仪。

## 库安装
这里提供两种使用本库的方法：<br>
1.打开Arduino IDE,在状态栏中的Tools--->Manager Libraries 搜索"DFRobot_Alcohol"并安装本库.<br>
2.首先下载库文件,将其粘贴到\Arduino\libraries目录中,然后打开examples文件夹并在该文件夹中运行演示.<br>

## 方法

```C++
 /**
  * @fn mySerial
  * @brief 初始化串口
  * @param RX 接收 io口号
  * @param TX 发送 io口号
  * @return None
  */
   mySerial(RX, TX);

  /**
   * @fn setModes
   * @brief 设置传感器模式
   * @param mode mode is 主动模式或者问答模式
   * @n       MEASURE_MODE_AUTOMATIC    主动模式
   * @n       MEASURE_MODE_PASSIVE      问答模式
   * @return None
   */
  void setModes(uint8_t mode);

  /**
   * @fn readAlcoholData
   * @brief 读取酒精弄浓度数据
   * @param collectNum is 平滑数据参数，默认采集个数20
   * @n       COLLECT_NUMBER       采集个数
   * @return 酒精浓度  单位(PPM).
   */
  float readAlcoholData(uint8_t collectNum = 20);
```

## 兼容性

| 主板        | 通过 | 未通过 | 未测试 | 备注 |
| ----------- | :--: | :----: | :----: | ---- |
| Arduino uno |  √   |        |        |      |
| Mega2560    |  √   |        |        |      |
| Leonardo    |  √   |        |        |      |
| ESP32       |  √   |        |        |      |
| micro:bit   |      |        |   √    |      |


## 历史

- 2020/9/9 - V1.0.0 版本

## 创作者

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2020. (Welcome to our [website](https://www.dfrobot.com/))