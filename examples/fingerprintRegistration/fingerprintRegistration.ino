/*!
 * @file fingerprintRegistration.ino
 * @brief Fingerprint Acquisition and Saving 
 * @details This module can be controlled by hardware serial or software serial 
 * @n Experiment Phenomenon：auto retrieve unregistered ID, collect fingerprint 3 times.  
 * @n           In collecting, set LED ring to breathing lighting in blue, and then to quick blink in yellow 3 times when completed 
 * @n           At last, save the fingerprint in an unregistered ID, the green LED lights up for 1s and turns off. 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.1
 * @date  2020-03-19
 * @url https://github.com/DFRobot/DFRobot_ID809_I2C
*/

#include <DFRobot_ID809_I2C.h>

#define COLLECT_NUMBER 3  //Fingerprint sampling times, can be set to 2-3

DFRobot_ID809_I2C fingerprint;
//String desc;

void setup(){
  /*Init print serial port */
  Serial.begin(9600);
  /*Take FPSerial as communication port of the module*/
  fingerprint.begin();
  /*Wait for Serial to open*/
  while(!Serial);
  /*Test whether the device can communicate properly with mainboard 
    Return true or false
    */
  while(fingerprint.isConnected() == false){
    Serial.println("Communication with device failed, please check connection");
    /*Get error code information*/
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
    delay(1000);
  }
}

uint8_t ID,i,ret;

void loop(){
  /*Get an unregistered ID for saving fingerprint 
    Return ID when succeeded 
    Return ERR_ID809 if failed 
   */
  if((ID = fingerprint.getEmptyID()) == ERR_ID809){
    while(1){
      /*Get error code information*/
      //desc = fingerprint.getErrorDescription();
      //Serial.println(desc);
      delay(1000);
    }
  }
  Serial.print("unresistered ID,ID=");
  Serial.println(ID);
  i = 0;   //Clear sampling times 
  /*Fingerprint sampling 3 times*/
  while(i < COLLECT_NUMBER){
    /*Set fingerprint LED ring mode, color, and number of blinks 
      Can be set as follows: 
      Parameter 1:<LEDMode>
      eBreathing   eFastBlink   eKeepsOn    eNormalClose
      eFadeIn      eFadeOut     eSlowBlink   
      Parameter 2:<LEDColor>
      eLEDGreen  eLEDRed      eLEDYellow   eLEDBlue
      eLEDCyan   eLEDMagenta  eLEDWhite
      Parameter 3:<Number of blinks> 0 represents blinking all the time 
      This parameter will only be valid in mode eBreathing, eFastBlink, eSlowBlink
     */
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eBreathing, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
    Serial.print("The fingerprint sampling of the");
    Serial.print(i+1);
    Serial.println("(th) is being taken");
    Serial.println("Please press down your finger");
    /*Capture fingerprint image, 10s idle timeout 
      IF succeeded, return 0, otherwise, return ERR_ID809
     */
    if((fingerprint.collectionFingerprint(/*timeout = */10)) != ERR_ID809){
      /*Set fingerprint LED ring to quick blink in yellow 3 times */
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3);
      Serial.println("Sampling succeeds");
      i++;   //Sampling times +1
    }else{
      Serial.println("Sampling failed");
      /*Get error code information*/
      //desc = fingerprint.getErrorDescription();
      //Serial.println(desc);
    }
    Serial.println("Please release your finger");
    /*Wait for finger to release 
      Return 1 when finger is detected, otherwise return 0 
     */
    while(fingerprint.detectFinger());
  }
  
  /*Save fingerprint in an unregistered ID */
  if(fingerprint.storeFingerprint(/*Empty ID = */ID) != ERR_ID809){
    Serial.print("Saving succeed,ID=");
    Serial.println(ID);
    Serial.println("-----------------------------");
    /*Set fingerprint LED ring to always ON in green */
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
    delay(1000);
    /*Turn off fingerprint LED ring */
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eNormalClose, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
    delay(1000);
  }else{
    Serial.println("Saving failed");
    /*Get error code information*/
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
  }
}
