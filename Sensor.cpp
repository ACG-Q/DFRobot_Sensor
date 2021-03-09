#include "Sensor.h"

Sensor::Sensor(uint8_t mode)
{
  switchMode(mode);
};

int Sensor::begin(void) {
  uint8_t id = 0;
  if (readReg(REG_READ_INFO, &id, 1) == 0) {
      return ERR_DATA_BUS;
  }
  if(id != IIC_ID){
    return ERR_IC_VERSION;
  }
  return ERR_OK;
}

void Sensor::setLED(uint8_t b, uint8_t g, uint8_t r)
{
  sColor_t color;
  color = {.b= b >> 3, .g= g >> 2, .r= r >> 3};
  writeReg(REG_COLOR_LED, &color, sizeof(color));
};

void Sensor::setLED(uint16_t color)
{
  writeReg(REG_COLOR_LED, &color, sizeof(color));
};

void Sensor::readLightIntensity(void)
{
  uint8_t *pLight = _pLight;
  readReg(REG_LIGHT_LED,&pLight,8);
};


void Sensor::switchMode(uint8_t &mode)
{
  writeReg(REG_MODE, &mode, sizeof(mode));
};




Sensor_IIC::Sensor_IIC(uint8_t mode=NORMAL_POWER_MODE,Wire &Wire):
Sensor(mode){
  _deviceAddr = IIC_ADDR;
  _pWire = Wire;
};

int Sensor_IIC::begin()
{
  _pWire.begin();
  return Sensor::begin();
};

void Sensor_IIC::writeReg(uint8_t Reg, void* data,size_t s)
{
  uint8_t *_data = (uint8_t *)data;
  _pWire.beginTransmission(_deviceAddr);
  _pWire.write(&Reg);
  for(uint16_t i=0; i<s; i++){
    _pWire.write(_data[i]);
  }
  _pWire.endTransmission();
};

uint16_t Sensor_IIC::readReg(uint8_t Reg, void* data, size_t s)
{
  uint8_t* _data = (uint8_t *)data;
  uint16_t count = 0;
  _pWire.beginTransmission(_deviceAddr);
  _pWire.write(&Reg);
  _pWire.endTransmission();
  _pWire.requestFrom(_deviceAddr, (uint8_t)s, (uint8_t)TRUE);
  for(uint16_t i=0; i<s; i++){
    *_data++ = _pWire.read();
    count++;
  }
  return count;
};


Sensor_SPI::Sensor_SPI(uint16_t speedMaximum=14000000,
                      uint16_t dataOrder=MSBFIRST,
                      uint8_t dataMode=SPI_MODE0,
                      uint8_t mode=NORMAL_POWER_MODE,
                      uint16_t baudRate=115200,
                      uint8_t pin):
Sensor(mode)
{
  _SPI = SPISettings(speedMaximum, dataOrder, dataMode);
  _baudRate = baudRate;
  _sPin = pin;
};

void Sensor_SPI::begin()
{
  Serial.begin(_baudRate);
  pinMode(_sPin,OUTPUT);
  SPI.begin();
  return Sensor::begin();
};

void Sensor_SPI::writeReg(uint8_t Reg, void* data, size_t s)
{
  uint8_t *_data = (uint8_t *)data;
  SPI.beginTransaction(_SPI);
  digitalWrite(_sPin,LOW);
  SPI.transfer(&Reg);
  while(s--){
    SPI.transfer(*_data++);
  }
  digitalWrite(_sPin,HIGH);
  SPI.endTransaction();
};

uint16_t Sensor_SPI::readReg(uint8_t Reg, void* data, size_t s)
{
  uint8_t* _data = (uint8_t *)data;
  uint16_t count=0;
  SPI.beginTransaction(_SPI);
  digitalWrite(_sPin,LOW);
  SPI.transfer(&Reg);
  while(s--){
    *_data++ = SPI.transfer(0x00);
    count++;
  }
  digitalWrite(_sPin,HIGH);
  SPI.endTransaction();
  return count;
};
