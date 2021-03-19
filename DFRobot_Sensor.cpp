/*!
 * @file DFRobot_Sensor.cpp
 * @brief (仿写)定义DFRobot_Sensor 类的基础结构，基础方法的实现
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [liuji](liuji.liu@dfrobot.com)
 * @version  V1.0
 * @date  2021-03-10
 * @url https://github.com/acg-q/DFRobot_Sensor
 */
#include "DFRobot_Sensor.h"

DFRobot_Sensor::DFRobot_Sensor(uint8_t mode):
_mode(mode){
  
};

uint16_t DFRobot_Sensor::begin()
{
  uint8_t id;
  if(readReg(SENSOR_READ_CONFIG_ADDR,&id,1) != 0){
    return ERR_DATA_BUS;
  };
  if(id != ID){
    return ERR_IC_VERSION;
  };
  writeReg(SENSOR_WRITE_CONFIG_ADDR, &_mode, sizeof(_mode));
  return ERR_OK;
};

void DFRobot_Sensor::setLED(uint8_t r,uint8_t g,uint8_t b)
{
  sColor_t color={
    .g=g>>2,
    .b=b>>3,
    .r=r>>3
  };
  writeReg(SENSOR_COLOR_LED_ADDR, &color, sizeof(color));
};

void DFRobot_Sensor::setLED(uint16_t color)
{
  writeReg(SENSOR_COLOR_LED_ADDR, &color, sizeof(color));
};

uint16_t DFRobot_Sensor::soundStrengthDB()
{
  sCombinedData_t data;
  readReg(SENSOR_READ_VALUE_ADDR,&data,1);
  return data.sound;
};

uint16_t DFRobot_Sensor::lightStrengthLux()
{
  sCombinedData_t data;
  readReg(SENSOR_READ_VALUE_ADDR,&data,1);
  return data.light;
};

bool DFRobot_Sensor::switchMode(uint8_t mode)
{
  uint8_t tmp1,tmp2;
  readReg(SENSOR_WRITE_CONFIG_ADDR,&tmp1,1);
  writeReg(SENSOR_WRITE_CONFIG_ADDR,&mode,sizeof(mode));
  readReg(SENSOR_WRITE_CONFIG_ADDR,&tmp2,1);
  if(tmp1 == tmp2){
    return false;
  };
  return true;
};


DFRobot_Sensor_IIC::DFRobot_Sensor_IIC(TwoWire *pWire, uint8_t mode):
DFRobot_Sensor::DFRobot_Sensor(mode)
{
  _pWire = pWire;
  _deviceAddr = IIC_ADDR;
};

uint16_t DFRobot_Sensor_IIC::begin(){
  _pWire -> begin();
  return DFRobot_Sensor::begin();
};

void DFRobot_Sensor_IIC::writeReg(uint8_t reg,void* pData,size_t quantity){
  uint8_t* _pData = (uint8_t *)pData;
  uint16_t _size = (uint16_t)quantity;
  _pWire -> beginTransmission(_deviceAddr);
  _pWire -> write(&reg,1);
  while(_size --){
    _pWire -> write(*_pData++);
  };
  _pWire -> endTransmission();
};

uint16_t DFRobot_Sensor_IIC::readReg(uint8_t reg,void* pBuf,size_t quantity){
  uint8_t* _pBuf = (uint8_t *)pBuf;
  uint16_t _size = (uint16_t)quantity;
  uint16_t count = 0;
  _pWire -> beginTransmission(_deviceAddr);
  _pWire -> write(&reg,1);
  _pWire -> endTransmission();
  _pWire -> requestFrom(_deviceAddr,_size);
  for(uint16_t i=0; i<_size; i++){
    _pBuf[i] = _pWire -> read();
    count++;
  };
  return count;
};

DFRobot_Sensor_SPI::DFRobot_Sensor_SPI(SPIClass* pSpi, uint8_t mode, uint8_t csPin):
DFRobot_Sensor::DFRobot_Sensor(mode){
  _csPin = (uint8_t)csPin;
  _pSpi = pSpi;
};

uint16_t DFRobot_Sensor_SPI::begin(){
  pinMode(_csPin, OUTPUT);
  _pSpi -> begin();
  return DFRobot_Sensor::begin();
};

void DFRobot_Sensor_SPI::writeReg(uint8_t reg,void* pData,size_t quantity){
  uint8_t * _pData = (uint8_t*) pData;
  uint16_t _size = (uint16_t)quantity;
  _pSpi -> beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  _pSpi -> transfer(reg);
  digitalWrite(_csPin,LOW);
  for(uint16_t i = 0; i< _size; i++){
    _pSpi -> transfer(*_pData++);
  };
  digitalWrite(_csPin,HIGH);
  _pSpi -> endTransaction();
};

uint16_t DFRobot_Sensor_SPI::readReg(uint8_t reg,void* pBuf,size_t quantity){
  uint8_t * _pData = (uint8_t*) pBuf;
  uint16_t _size = (uint16_t)quantity;
  uint16_t count = 0;
  _pSpi -> beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  _pSpi -> transfer(reg);
  digitalWrite(_csPin,LOW);
  for(uint16_t i = 0; i< _size; i++){
    _pData = _pSpi -> transfer(0x00);
    count++;
  };
  digitalWrite(_csPin,HIGH);
  _pSpi -> endTransaction();
  return count;
};
