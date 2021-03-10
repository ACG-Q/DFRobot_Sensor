/*!
 * @file switchMode.ino
 * @brief 各种模式切换
 * @n 实验现象：演示各种模式切换，保证板子正常工作
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

DFRobot_Sensor_SPI sensor(&SPI, 13, ,mode);

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
  /*设置模式*/
  int ret;
  // 切换到正常功耗模式, 
  if((ret = sensor.switchMode(sensor.eNormalPower)) != 0){
    Serial.print("切换到eNormalPower失败 ret=");
    Serial.println(ret);
  }
  // 切换到高速度模式
  if((ret = sensor.switchMode(sensor.eHighSpeed)) != 0){
    Serial.print("切换到eNormalPower失败 ret=");
    Serial.println(ret);
  }
  // 切换到正常速度模式
  if((ret = sensor.switchMode(sensor.eNormalSpeed)) != 0){
    Serial.print("切换到eNormalPower失败 ret=");
    Serial.println(ret);
  }
  // 切换到低功耗模式
  if((ret = sensor.switchMode(sensor.eLowPower)) != 0){
    Serial.print("切换到eNormalPower失败 ret=");
    Serial.println(ret);
  }
  // 切换到eNormalPower+eHighSpeed模式
  if((ret = sensor.switchMode(sensor.eNormalPower+sensor.eHighSpeed)) != 0){
    Serial.print("切换到eNormalPower+eHighSpeed失败 ret=");
    Serial.println(ret);
  }
  // 切换到eHighPrecision+eNormalSpeed模式
  if((ret = sensor.switchMode(sensor.eHighPrecision+sensor.eNormalSpeed)) != 0){
    Serial.print("切换到eHighPrecision+eNormalSpeed失败 ret=");
    Serial.println(ret);
  }
}