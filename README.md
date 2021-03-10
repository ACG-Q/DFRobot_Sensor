# DFRobot_Sensor
数据手册上抄一下芯片的描述

这里写模块介绍，做到读完这段，就能对模块有初步了解，让客户懂的用这个模块能干什么（数据手册通常比较官方，这里你可以举例子，更场景化）<br>
这个模块的优点，告诉用户为什么要购买这个模块。一些关键术语，我们要在readme中有解释

这里需要显示拍照图片，可以一张图片，可以多张图片（不要用SVG图）

![正反面svg效果图](resources/images/SEN0245svg1.png)


## 产品链接（链接到英文商城）
    SKU：虚构的产品
   
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

- 设置LED灯颜色
- 读取光线强度DB
- 读取声音强度Lux
- 切换模式

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /**
   * @brief 初始化函数
   * @return 返回0表示初始化成功，返回其他值表示初始化失败
   */
  uint16_t begin();

  /**
   * @brief 设置LED灯的颜色
     @note  设置颜色后，0.2秒后生效
   * @param r 红色通道颜色值，范围0-255
   * @param g 绿色通道颜色值，范围0-255
   * @param b 蓝色通道颜色值，范围0-255
   */
  void setLED(uint8_t r,uint8_t g,uint8_t b);

  /**
   * @brief 设置LED灯的颜色
     @note  设置颜色后，0.2秒后生效
   * @param color rgb565格式的颜色值
   */
  void setLED(uint16_t color);

   /**
   * @brief 获取声音强度值
   * @return 返回声音强度，单位是DB
   */
  uint16_t soundStrengthDB();
  
  /**
   * @brief 获取光线强度值
   * @return 返回光线强度，单位是勒克斯(Lux)
   */
  uint16_t lightStrengthLux();

  /**
   * @brief 切换模式
   * @param mode 可以是eLowPowerMode_t和eHighSpeedMode_t类型和ePrecisionMode_t;也可以是三者组合
   * @return 返回0操作成功, 返回其他值操作失败
   */
  uint16_t switchMode(uint8_t mode);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32           |      √       |              |             | 
micro:bit        |      √       |              |             | 


## History

- data 2021-3-10
- version V1.0


## Credits

Written by Alexander(ouki.wang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))





