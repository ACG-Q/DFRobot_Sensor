/*!
 * @file readLight.ino
 * @brief 读取环境光线强度，单位是勒克斯(Lux)
 * @n 实验现象：每秒读取一次环境光线强度，并打印到串口，给传感器以不同的光照强度，可以得到不同的采集结果
 * @n 由于只有4 bits存放光照强度，最低分辨率1000，所以能采集的范围是 0Lux-150000Lux
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [ll](ll@dfrobot.com)
 * @version  V1.0
 * @date  2021-10-10
 * @get from https://www.dfrobot.com
 * @url https://github.com/acg-q/DFRobot_Sensor
 */
#include <DFRobot_Sensor.h>

sMode_t mode = {
  .lowpower = eLowPower,
  .highspeed = eHighSpeed,
  .precision = eNomalPrecision,
  .reserved = eLowPower,
  .ready = eLowPower
};

DFRobot_Sensor_IIC sensor(&Wire, mode);

void setup(void)
{
  /*在这里一致等到芯片初始化完成才能退出*/
  while(sensor.begin(115200) != 0){
    Serial.println("初始化芯片失败，请确认芯片连接是否正确");
    delay(1000);
  }
}

void loop(void)
{
  /*读取光线强度*/
  uint16_t lightStrength = sensor.lightStrengthLux();
  Serial.print("Light Strength=");
  Serial.print(lightStrength);
  Serial.println(" Lux");
  delay(1000);
}