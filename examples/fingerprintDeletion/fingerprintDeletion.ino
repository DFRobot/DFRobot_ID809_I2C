/*!
 * @file fingerprintDeletion.ino
 * @brief Delete a specific fingerprint 
 * @details Experiment phenomenon：press your finger on the sensor, if this fingerprint is registered, delete it and LED turns on in green. 
      If it is unregistered or fingerprint collection fails, LED light turns on in red.
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
  /*Init print serial port*/
  Serial.begin(9600);
  /*Take FPSerial as communication serial port of the fingerprint module*/
  fingerprint.begin();
  /*Wait for Serial to open*/
  while(!Serial);
  /*Test whether the device can properly communicate with mainboard
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

void loop(){
  uint8_t ret = 0;
  Serial.println("Press your finger on the sensor to delete the fingerprint");
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
  /*Capture fingerprint image, 10s idle timeout 
    If succeed return 0, otherwise return ERR_ID809
   */
  if((fingerprint.collectionFingerprint(/*timeout=*/10)) != ERR_ID809){
    /*Compare the captured fingerprint with all the fingerprints in the fingerprint library 
      Return fingerprint ID(1-80) if succeed, return 0 when failed 
     */
    ret = fingerprint.search();
    if(ret != 0){
      /*Delete the fingerprint of this ID*/
      fingerprint.delFingerprint(/*Fingerprint ID = */ret);
      //fingerprint.delFingerprint(DELALL);  //Delete all fingerprints 
      Serial.print("delete succeeds,ID=");
      Serial.println(ret);
      /*Set fingerprint LED ring to always ON in green */
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
    }else{
      Serial.println("Fingerprint is unregistered");
      /*Set fingerprint LED ring to always ON in red*/
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */0);
    }
  }else{
    Serial.println("Capturing fails");
    /*Get error code information*/
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
    /*Set fingerprint LED ring to always ON in red*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */0);
  }
  Serial.println("Please release your finger");
  /*Wait for finger to release
    Return 1 when finger is detected, otherwise return 0 
   */
  while(fingerprint.detectFinger());
    /*Check whether the fingerprint ID has been registered 
    Return 1 if registered, otherwise return 0 
   */
//  if(fingerprint.getStatusID(/*Fingerprint ID = */ret)){
//    Serial.println("ID has been registered");
//  }else{
//    Serial.println("ID is unregistered");
//  }
  Serial.println("-----------------------------");
  delay(1000);
}
