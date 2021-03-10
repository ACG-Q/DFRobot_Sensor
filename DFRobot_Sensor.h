/*!
 * @file DFRobot_Sensor.h
 * @brief (仿写)定义DFRobot_Sensor 类的基础结构
 * @n 这是一个虚拟的传感器，IIC地址不可改变,不对应任何实物，可以通过IIC和SPI口来控制它，假设它有下面这些功能
 * @n 向寄存器0里写入数据，点亮不同颜色的LED灯
 * @n 从寄存器1里读出数据，高四位表示光线强度，低四位表示声音强度
 * @n 从寄存器2 bit0里写入数据，写1表示正常模式，写0表示低功耗模式
 * @n 从寄存器3 读取数据，读到的是芯片版本0xDF
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Ouki](ouki.wang@dfrobot.com)
 * @version  V1.0
 * @date  2021-03-10
 * @url https://github.com/acg-q/DFRobot_Sensor
 */
#ifndef __DFROBOT_SENSOR_H
#define __DFROBOT_SENSOR_H

#includes <Arduino.h>
#includes <Wire.h>
#includes <SPI.h>

// 颜色
#define  COLOR_RGB565_BLACK     0x0000      // 黑色    
#define  COLOR_RGB565_NAVY      0x000F      // 深蓝色  
#define  COLOR_RGB565_DGREEN    0x03E0      // 深绿色  
#define  COLOR_RGB565_DCYAN     0x03EF      // 深青色  
#define  COLOR_RGB565_PURPLE    0x780F      // 紫色  
#define  COLOR_RGB565_MAROON    0x7800      // 深红色      
#define  COLOR_RGB565_OLIVE     0x7BE0      // 橄榄绿      
#define  COLOR_RGB565_LGRAY     0xC618      // 灰白色
#define  COLOR_RGB565_DGRAY     0x7BEF      // 深灰色      
#define  COLOR_RGB565_BLUE      0x001F      // 蓝色    
#define  COLOR_RGB565_GREEN     0x07E0      // 绿色          
#define  COLOR_RGB565_CYAN      0x07FF      // 青色  
#define  COLOR_RGB565_RED       0xF800      // 红色       
#define  COLOR_RGB565_MAGENTA   0xF81F      // 品红    
#define  COLOR_RGB565_YELLOW    0xFFE0      // 黄色        
#define  COLOR_RGB565_WHITE     0xFFFF      // 白色  

// 寄存器地址
#define SENSOR_COLOR_LED_ADDR       0 //LED控制地址(16bits) 设置LED灯颜色
#define SENSOR_READ_VALUE_ADDR      1 //组合数据地址(8bits) 读取光线、声音强度
#define SENSOR_WRITE_CONFIG_ADDR    2 //配置寄存器地址(8bits) 写入数据
#define SENSOR_READ_CONFIG_ADDR     3 //芯片ID寄存器地址(8bits) 读取数组

// IIC地址、芯片ID
#define IIC_ADDR        0x09
#define ID              0x99

class DFRobot_Sensor{
public:
#define ERR_OK             0      //无错误
#define ERR_DATA_BUS      -1      //数据总线错误
#define ERR_IC_VERSION    -2      //芯片版本不匹配

  /* 
    这里从数据手册上抄写关于这个寄存器的描述
   * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   * |    b15   |    b14   |    b13   |    b12   |    b11   |    b10   |    b9     |    b8    |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
   * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   * |                         b                            |                                 g                                |                           r                           |
   * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   */
  typedef struct{
    uint8_t g:  5,
    uint8_t b:  6,
    uint8_t r:  5
  }__attribute__ ((packed)) sColor_t;
  
  /*
   这里从数据手册上抄写关于这个寄存器的描述
     * ------------------------------------------------------------------------------------------
     * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
     * ------------------------------------------------------------------------------------------
     * |                 声音强度                  |                  光线强度                  |
     * ------------------------------------------------------------------------------------------
   */
  typedef struct {
    uint8_t   light:  4;
    uint8_t   sound:  4;
  } __attribute__ ((packed)) sCombinedData_t;
  
  /*
   这里从数据手册上抄写关于这个寄存器的描述
     * -----------------------------------------------------------------------------------------
     * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2   |    b1     |    b0    |
     * -----------------------------------------------------------------------------------------
     * |   ready  |         reversed               |      precision     | highspeed | lowpower |
     * -----------------------------------------------------------------------------------------
     *
     *上电后，ready位默认为1，不可更改
  */
  typedef struct {
    uint8_t   lowpower:  1;   /*!< 上电为0，1：低功耗模式 0：正常功耗模式 */
    uint8_t   highspeed:  1;  /*!< 上电为0，1：高速模式 0：正常速度模式 */
    uint8_t   precision:  2;  /*!< 上电为0，0：低精度模式，1：正常精度模式，2：高精度模式，3：超高精度模式 */
    uint8_t   reserved:  4;   /*!< 上电为0，1：低功耗模式 0：正常功耗模式 */
    uint8_t   ready:  1;      /*!< 上电为0，1：低功耗模式 0：正常功耗模式 */
  } __attribute__ ((packed)) sMode_t;
  
  /*
  从数据手册上抄写
  */
  typedef enum{
    eNormalPower = 0, /**< 正常功耗模式，功耗范围20mW-60mW，可以搭配任意采集速度eSpeedMode_t和采集精度ePrecisionMode_t */
    eLowPower = 1,    /**< 低功耗模式，功耗范围2mW-4mW, 注意在低功耗模式下，采集速度eSpeedMode_t只能搭配eNormalSpeed，采集精度ePrecisionMode_t只能搭配eLowPrecision和eNomalPrecision*/
  }eLowPowerMode_t;
  
  /*
  从数据手册上抄写
  */
  typedef enum{
    eNormalSpeed = 0<<1, /**< 正常采集速度，可以和任意精度搭配使用 */
    eHighSpeed = 1<<1,   /**< 高速采集模式，采集周期10ms，可以进入低功耗，可以配置为eLowPrecision和eNomalPrecision两种精度模式 */
  }eSpeedMode_t;
  
  /*
  从数据手册上抄写
  */
  typedef enum{
    eLowPrecision   = 0<<2, /**< 低精度，精度大概在xxx，在低精度模式下，可以进入低功耗 */
    eNomalPrecision = 1<<2, /**< 正常精度，精度大概在xxx，在正常精度模式下，可以进入低功耗 */
    eHighPrecision  = 2<<2, /**< 高精度，精度大概在xxx，在高精度模式下，采集速率会降低，采集周期100ms，不能进入低功耗 */
    eUltraPrecision = 3<<2, /**< 超高精度，精度大概在xxx，在超高精度模式下，采集速率会极低，采集周期1000ms，不能进入低功耗 */
  }ePrecisionMode_t;
  
public:
  DFRobot_Sensor(uint8_t mode=eNomalPrecision+eNormalSpeed+eNormalPower);
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
  
protected:

   /**
   * @brief 写寄存器函数，设计为虚函数，由派生类实现函数体
   * @param reg  寄存器地址 8bits
   * @param pdata 要写入数据的存放缓存
   * @param quantity 要写入数据的长度
   */
  virtual void writeReg(uint8_t reg,void* pData,size_t quantity) = 0;
  
  /**
   * @brief 读取寄存器函数，设计为虚函数，由派生类实现函数体
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param quantity 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  virtual uint16_t readReg(uint8_t reg,void* pBuf,size_t quantity) = 0;

private:
  uint8_t _mode;
};

class DFRobot_Sensor_IIC::public DFRobot_Sensor
{
public:
  DFRobot_Sensor_IIC(TwoWire *_pWire=&Wire, uint8_t mode);
  
  /**
   * @brief 初始化函数
   * @param speed 波特率
   * @return 返回0表示初始化成功，返回其他值表示初始化失败
   */
  uint16_t begin(uint16_t speed);

protected:
  
  /**
   * @brief 通过IIC总线写入寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  virtual void writeReg(uint8_t reg,void* pData,size_t quantity);
  
  /**
   * @brief 通过IIC总线读取寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  virtual uint16_t readReg(uint8_t reg,void* pBuf,size_t quantity);

private:
  TwoWire *_pWire;
  uint8_t _deviceAddr;
};


class DFRobot_Sensor_SPI::public DFRobot_Sensor
{
public:
  DFRobot_Sensor_SPI(uint8_t mode, uint8_t csPin);
  
  /**
   * @brief 初始化函数
   * @param speed 波特率
   * @return 返回0表示初始化成功，返回其他值表示初始化失败
   */
  uint16_t begin(uint16_t speed);

protected:
  
  /**
   * @brief 通过SPI总线写入寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param quantity 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  virtual void writeReg(uint8_t reg,void* pData,size_t quantity);
  
  /**
   * @brief 通过SPI总线写入寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param quantity 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  virtual uint16_t readReg(uint8_t reg,void* pBuf,size_t quantity);

private:
  uint8_t _csPin;
};

#endif