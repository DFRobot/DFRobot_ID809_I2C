# DFRobot_ID809_I2C
- [中文版](./README_CN.md)

This capacitive fingerprint sensor supports fingerprint capture, image processing, fingerprint storage, fingerprint comparison and so on.
Taking ID809 high-performance processor and semiconductor fingerprint sensor as the core, the sensor adopts built-in IDfinger6.0 algorithm, which can complete all fingerprint identification work independently.<br>

The product supports UART communication. When working with Arduino library, it can easily realize functions like  fingerprint registration, fingerprint deletion, etc.<br>

This fingerprint sensor comes with round breathing LEDs and has a simple structure, small size, and delicate appearance. The sensor offers fast recognition speed and high security. What’s more, it supports 360-degree arbitrary angle recognition and deep self-learning function, high performance and low power consumption.

![Product Image](./resources/images/SEN0348.png)

## Product Link (https://www.dfrobot.com/product-2051.html)
    SEN0348: Capacitive Fingerprint Sensor / Scanner

   

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

1. Fingerprint Registration <br>
2. Fingerprint Matching <br>
3. Fingerprint Deletion <br>
4. Supports for sleep mode <br>

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /**
   * @fn begin
   * @brief Init communication port
   * @return Boolean type, the result of measure
   * @retval 初始化成功
   * @retval 初始化失败
   */
  virtual bool begin(void) = 0;
  
  /**
   * @fn isConnected
   * @brief Test whether the module connection is ok
   * @return Boolean type, the result of measure
   * @retval 连接成功
   * @retval 连接失败
   */
  bool isConnected();
  
  /**
   * @fn setDeviceID
   * @brief Set module ID
   * @param deviceID 1-255
   * @return 设置的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t setDeviceID(uint8_t deviceID);
  
  /**
   * @fn setSecurityLevel
   * @brief Set module security level 
   * @param securityLevel 1-5
   * @return 设置的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t setSecurityLevel(uint8_t securityLevel);
  
  /**
   * @fn setDuplicationCheck
   * @brief Set module fingerprint replication check (Check whether the fingperint has been registered when saving it)
   * @param duplicationCheck 1(ON) or 0(OFF)
   * @return 设置的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t setDuplicationCheck(uint8_t duplicationCheck);
  
  /**
   * @fn setBaudrate
   * @brief Set module baud rate 
   * @param baudrate in typedef enum eDeviceBaudrate_t
   * @return 设置的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t setBaudrate(eDeviceBaudrate_t baudrate);
  
  /**
   * @fn setSelfLearn
   * @brief Set module self-learning function (Update fingeprint when fingerprint comparison succeeds)
   * @param selfLearn 1(ON) or 0(OFF)
   * @return 设置的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t setSelfLearn(uint8_t selfLearn);
  
  /**
   * @fn getDeviceID
   * @brief Read module ID
   * @return 获取的指纹ID
   * @retval ID号:1-255
   * @retval ERR_ID809 错误
   */
  uint8_t getDeviceID();
  
  /**
   * @fn getSecurityLevel
   * @brief Read module security level 
   * @return 安全等级编号
   * @retval Security level:1-5
   * @retval ERR_ID809 错误
   */
  uint8_t getSecurityLevel();
  
  /**
   * @fn getDuplicationCheck
   * @brief Read module fingerprint replication check status
   * @return 检测结果
   * @retval 1 ON
   * @retval 0 OFF
   * @retval ERR_ID809 错误
   */
  uint8_t getDuplicationCheck();
  
  /**
   * @fn getBaudrate
   * @brief Read module baud rate 
   * @return Baudrate in typedef enum eDEVICE_BAUDRATE_t or ERR_ID809
   */
  uint8_t getBaudrate();
  
  /**
   * @fn getSelfLearn
   * @brief Read module self-learning function status 
   * @return 检测结果
   * @retval 1 ON
   * @retval 0 OFF
   * @retval ERR_ID809 错误
   */
  uint8_t getSelfLearn();
   
  /**
   * @fn getDeviceInfo
   * @brief Read device number 
   * @return Device number
   */
  String getDeviceInfo();
  
  /**
   * @fn setModuleSN
   * @brief Set serial number
   * @param SN String pointer 
   * @return 设置的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t setModuleSN(const char* SN);
  
  /**
   * @fn getModuleSN
   * @brief Read serial number 
   * @return Serial number
   */
  String getModuleSN();
  
  /**
   * @fn ctrlLED
   * @brief Set LED
   * @param mode in typedef enum eLEDMode_t
   * @param color in typedef enum eLEDColor_t
   * @param blinkCount 00 represents blinking all the time
   * @n This parameter will only be valid in mode eBreathing, eFastBlink, eSlowBlink
   * @return 设置的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t ctrlLED(eLEDMode_t mode,eLEDColor_t color,uint8_t blinkCount);
  
  /**
   * @fn detectFinger
   * @brief Detect if there is finger touched 
   * @return 检测的结果
   * @retval 1 Touched 
   * @retval 0 No touch 
   */
  uint8_t detectFinger();
  
  /**
   * @fn getEmptyID
   * @brief Get the first registerable ID 
   * @return Registerable ID 
   */
  uint8_t getEmptyID();
  
  /**
   * @fn getStatusID
   * @brief Check if the ID has been registered 
   * @param ID
   * @return 状态
   * @retval 0 Registered 
   * @retval 1 Unregistered
   */
  uint8_t getStatusID(uint8_t ID);
  
  /**
   * @fn getEnrollCount
   * @brief Get the number of registered users 
   * @return Number of registered users 
   */
  uint8_t getEnrollCount();
  
  /**
   * @fn getEnrolledIDList
   * @brief Get registered user list 
   * @param list 已注册的ID列表
   * @return 获取的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
   uint8_t getEnrolledIDList(uint8_t* list);
  
  /**
   * @fn collectionFingerprint
   * @brief Fingerprint acquisition 
   * @param timeout 超时检测
   * @return 获取的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t collectionFingerprint(uint16_t timeout);
  
  /**
   * @fn storeFingerprint
   * @brief Save fingerprint 
   * @param ID Fingerprint ID
   * @return 操作的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t storeFingerprint(uint8_t ID);
  
  /**
   * @fn delFingerprint
   * @brief Delete fingerprint 
   * @param ID Delete ID or DELALL(delete all)
   * @return 操作的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t delFingerprint(uint8_t ID);
  
  /**
   * @fn search
   * @brief Match the fingerprint with all fingeprints 
   * @return Successfully matched fingerprint ID, 0(Matching failed)
   */
  uint8_t search();

  /**
   * @fn verify
   * @brief Match the fingerprint with specific fingerprint 
   * @brief ID
   * @return Successfully matched fingerprint ID, 0(Matching failed)
   */
  uint8_t verify(uint8_t ID);

  /**
   * @fn match
   * @brief Compare templates in two specific RamBuffers
   * @param RamBufferID0 number 
   * @param RamBufferID1 number 
   * @return 操作的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t match(uint8_t RamBufferID0, uint8_t RamBufferID1);
  
  /**
   * @fn getBrokenQuantity
   * @brief Get the number of damaged fingerprints 
   * @return Damaged fingerprint ID or ERR_ID809
   */
  uint8_t getBrokenQuantity();

  /**
   * @fn getBrokenID
   * @brief Get the first damaged fingerprint ID 
   * @return Damaged fingerprint ID 
   */
  uint8_t getBrokenID();
  
  /**
   * @fn loadFingerprint
   * @brief Take out fingerprint template, temporarily save into RamBuffer
   * @param ID Fingerprint ID 
   * @param RamBufferID number 0-2
   * @return 操作的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t loadFingerprint(uint8_t ID, uint8_t RamBufferID);
  
  /**
   * @fn enterSleepState
   * @brief Enter sleep mode
   * @return 操作的结果
   * @retval 0 succeed
   * @retval ERR_ID809 错误
   */
  uint8_t enterSleepState();
  
  /**
   * @fn getErrorDescription
   * @brief Get error information
   * @return Text description of error information
   */
  String getErrorDescription();
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32        |      √       |              |             | 
ESP8266        |      √       |              |             | 
M0        |      √       |              |             | 


## History

- 2021/12/14 - Version 1.0.0 released.

## Credits

Written by Eddard(Eddard.liu@dfrobot.com), 2020. (Welcome to our [website](https://www.dfrobot.com/))





