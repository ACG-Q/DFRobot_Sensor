#ifndef Sensor_h
#define Sensor_h
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#define FALSE 0
#define TRUE 1

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

#define  REG_COLOR_LED 0 // 寄存器0地址_LED灯颜色
#define  REG_LIGHT_LED 1 // 寄存器1地址_光线强度
#define  REG_MODE 2 // 寄存器2地址_功耗模式
#define  REG_READ_INFO 3 // 寄存器3地址_芯片版本

#define  LOW_POWER_MODE 0
#define  NORMAL_POWER_MODE 1
#define  HIGHT_POWER_MODE 2

#define  IIC_ADDR 0xFF // IIC设备地址
#define  IIC_ID 0xFF // IIC芯片ID

class Sensor
{
public:// 定义
#define ERR_OK             0      //无错误
#define ERR_DATA_BUS      -1      //数据总线错误
#define ERR_IC_VERSION    -2      //芯片版本不匹配
  // RGB结构体
  typedef struct{
    uint8_t b: 5;
    uint8_t g: 6;
    uint8_t r: 5;
  }__attribute__ ((packed)) sColor_t;
  // __attribute__ ((packed)) 保证结构体在不同的设备中大小相同
public:// 函数
  Sensor(uint8_t mode);
  /**
   * @brief 初始化函数
   * @return 返回0表示初始化成功，返回其他值表示初始化失败
   */
  int begin(void);
  /**
   * @brief 设置LED灯的颜色
   * @note  设置颜色后，0.2秒后生效
   * @param r 红色通道颜色值，范围0-255
   * @param g 绿色通道颜色值，范围0-255
   * @param b 蓝色通道颜色值，范围0-255
   */
  void setLED(uint8_t r, uint8_t g, uint8_t b);
  /**
   * @brief 设置LED灯的颜色
   * @note  设置颜色后，0.2秒后生效
   * @param color rgb565格式的颜色值
   */
  void setLED(uint16_t color);
  /**
   * @brief 获取光线强度值
   * @return 返回光线强度，单位是勒克斯(Lux)
   */
  void readLightIntensity(void);//读取光线强度
  /**
   * @brief 切换模式
   * @param mode 可以是eLowPowerMode_t和eHighSpeedMode_t类型和ePrecisionMode_t;也可以是三者组合
   * @return 返回0操作成功, 返回其他值操作失败
   */
  void switchMode(uint8_t &mode);//写入功耗模式
  
protected:
  /**
   * @brief 写寄存器函数，设计为虚函数，由派生类实现函数体
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   */
  virtual void writeReg(uint8_t Reg, void* data,size_t s);
  /**
   * @brief 读取寄存器函数，设计为虚函数，由派生类实现函数体
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  virtual uint16_t readReg(uint8_t Reg, void* data, size_t s);
};

class Sensor_IIC:public Sensor
{
public:
  uint8_t _deviceAddr;
  Wire _pWire;
  Sensor_IIC(uint8_t mode=NORMAL_POWER_MODE,Wire &Wire);
  /**
   * @brief 初始化函数
   * @return 返回0表示初始化成功，返回其他值表示初始化失败
   */
  int begin();
  /**
   * @brief 通过IIC总线写入寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  void writeReg(uint8_t Reg, void* data,size_t s);
  /**
   * @brief 通过IIC总线读取寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  uint16_t readReg(uint8_t Reg, void* data, size_t s);
};

class Sensor_SPI:public Sensor
{
public:
  uint8_t _SPI;
  uint16_t _baud;
  Sensor_SPI(uint16_t speedMaximum=14000000,
            uint16_t dataOrder=MSBFIRST,
            uint8_t dataMode=SPI_MODE0,
            uint8_t mode=NORMAL_POWER_MODE,
            uint16_t baudRate=115200,
            uint8_t pin);
  /**
   * @brief 初始化函数
   * @return 返回0表示初始化成功，返回其他值表示初始化失败
   */
  void begin();
  /**
   * @brief 通过IIC总线写入寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  void writeReg(uint8_t Reg, void* data,size_t s);
  /**
   * @brief 通过IIC总线读取寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  uint16_t readReg(uint8_t Reg, void* data, size_t s);
};

#endif
