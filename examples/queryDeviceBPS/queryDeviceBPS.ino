/*!
 * @file queryDeviceBPS.ino
 * @brief Query device baud rate
 * @n Experiment Phenomenon：Query device baud rate, and print from serial port 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_ID809
*/
#include <DFRobot_ID809.h>

uint32_t ID809_BPS[5] = {9600, 19200, 38400, 57600, 115200};

void setup(){
  uint8_t flag = 1;
  uint8_t i = 0;
  /*Init print serial port */
  Serial.begin(9600);
  /*Test module baud rate */
  do{
    /*Init FPSerial*/
    Serial.print(".");
    DFRobot_ID809_UART fingerprint(ID809_BPS[i]);
    /*Take FPSerial as communication port of fingerprint module */
    fingerprint.begin();
    if(fingerprint.isConnected() == false) flag = 0;
    i++;
    if(i > 4) i = 0;
  }while(flag);
  Serial.println(" ");
}

void loop(){
  Serial.print("Module baud rate:");
  Serial.println(ID809_BPS[i-1]);
  Serial.println("-----------------------------");
  delay(1000);
}
