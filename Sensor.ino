#include "Sensor.h"

Sensor_IIC sl2c;

void setup() {
  Serial.begin(115200);
  if(sl2c.begin() != 0){
    Serial.println("初始化芯片失败，请确认芯片连接是否正确");
    delay(1000);
  };
}

void loop() {
  sl2c.setLED(COLOR_RGB565_PURPLE);
  delay(1000);
  sl2c.setLED(COLOR_RGB565_YELLOW);
  delay(1000);
}
