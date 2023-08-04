/*!
 * @file getDeviceInformation.ino
 * @brief Get fingerprint module information 
 * @details Experiment Phenomenon：serial print module ID, security level, baud rate, etc. 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.1
 * @date  2020-03-19
 * @url https://github.com/DFRobot/DFRobot_ID809_I2C
*/

#include <DFRobot_ID809_I2C.h>

DFRobot_ID809_I2C fingerprint;
//String desc;

void setup(){
  /*Init print serial port */
  Serial.begin(9600);
  /*Take FPSerial as communication port of the module*/
  fingerprint.begin();
  /*Wait for Serial to open*/
  while(!Serial);
  /*Test whether device can communicate properly with mainboard 
    Return true or false
    */
  while(fingerprint.isConnected() == false){
    Serial.println("Communication with device failed, please check connection");
    /*Get error code information */
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
    delay(1000);
  }
}

uint8_t enrollCount;   //Number of registered users 

void loop(){
  /*Set module ID, available range: 1-255*/
  //fingerprint.setDeviceID(/*Device ID = */1);
  Serial.print("Module ID:");
  /*Read module ID*/
  Serial.println(fingerprint.getDeviceID());
  
  /*Set module security level, range 1-5. Default level: 3
    Security Level          Recognition rate 
      Level 1           FAR  0.1%
                        FRR  0.005%
      Level 2           FAR  0.003%
                        FRR  0 01%
      Level 3           FAR  0.001%
                        FRR  0.1%
      Level 4           FAR  0.003%
                        FRR  0.5%
      Level 5           FAR  0.0001%
                        FRR  1%
   */
  //fingerprint.setSecurityLevel(/*Security Level = */3);
  Serial.print("Module security level:");
  /*Read module security level*/
  Serial.println(fingerprint.getSecurityLevel());
  
  /*Set module baud rate, available range: 
    e9600bps    e19200bps   e38400bps   e57600bps    e115200bps
       1           2            3          4             5
   */
  //fingerprint.setBaudrate(fingerprint.e115200bps);
  Serial.print("Module baud rate: ");
  /*Read module baud rate*/
  Serial.println(fingerprint.getBaudrate());
  
  /*Set module self-learning function, 1(ON) 0(OFF)*/
  //fingerprint.setAutoLearn(/*Auto Learn = */1);
  Serial.print("Module self-learning function: ");
  /*Read the state of module self-learning function 
    Print ON if it is enabled, otherwise print OFF */
  Serial.println(fingerprint.getSelfLearn()?"ON":"OFF");
  
  /*Set module serial number, the number of characters of serial number should be small than 15 */
  //fingerprint.setModuleSN(/*Module SN = */"DFRobot");
  Serial.print("Module serial number:");
  /*Read module serial number */
  Serial.println(fingerprint.getModuleSN());
  
  Serial.print("Number of registered fingerprints in the module:");
  /*Get the number of registered users */
  Serial.println(enrollCount = fingerprint.getEnrollCount());
  /*Declare an array to hold ID list */
  uint8_t list[80] = {0};
  /*Get user list 
   */
  fingerprint.getEnrolledIDList(list);
  Serial.print("ID list of registered users: ");
  for(uint8_t i = 0; i < enrollCount; i++){
    Serial.print(list[i]);
    Serial.print(",");
  }
  
  Serial.print("\nNumber of broken fingerprints:");
  /*Get the number of broken fingerprints */
  Serial.println(fingerprint.getBrokenQuantity());
  /*Get the ID of the first broken fingerprint*/
  //fingerprint.getBrokenID();
  Serial.println("-----------------------------");

  delay(1000);
}
