/*!
 * @file fingerprintMatching.ino
 * @brief Gather fingerprint and compare it with fingerprints in fingerprint library
 * @details Experiment Phenomenon：capture fingerprint image and compare it with all fingerprints in the fingerprint library
                If matched successfully, light up green LED and print ID number. Return 0 when failed.
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
  /*Set fingerprint LED ring mode, color, and number of blinks
    Can be set as follows:
    Parameter 1:<LEDMode>
    eBreathing   eFastBlink   eKeepsOn    eNormalClose
    eFadeIn      eFadeOut     eSlowBlink   
    Parameter 2:<LEDColor>
    eLEDGreen  eLEDRed      eLEDYellow   eLEDBlue
    eLEDCyan   eLEDMagenta  eLEDWhite
    Parameter 3:<number of blinks> 0 represents blinking all the time
    This parameter will only be valid in mode eBreathing, eFastBlink, eSlowBlink
   */
  fingerprint.ctrlLED(/*LEDMode = */fingerprint.eBreathing, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
  Serial.println("Please press down your finger");
  /*Capture fingerprint image, 10s idle timeout 
    If succeed return 0, otherwise return ERR_ID809
   */
  if((fingerprint.collectionFingerprint(/*timeout=*/10)) != ERR_ID809){
    /*Set fingerprint LED ring to quick blink in yellow 3 times*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3);
    Serial.println("Capturing succeeds");
  }else{
    Serial.println("Capturing fails");
    /*Get error code information*/
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
  }
  Serial.println("Please release your finger");
  /*Wait for finger to release
    Return 1 when finger is detected, otherwise return 0 
   */
  while(fingerprint.detectFinger());
  
  /*Compare the captured fingerprint with all the fingerprints in the fingerprint library 
    Return fingerprint ID(1-80) if succeed, return 0 when failed 
   */
  ret = fingerprint.search();
  /*Compare the captured fingerprint with a fingerprint of specific ID 
    Return fingerprint ID(1-80) if succeed, return 0 when failed 
   */
  //ret = fingerprint.verify(/*Fingerprint ID = */1);  
  if(ret != 0){
    /*Set fingerprint LED ring to always ON in green */
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
    Serial.print("Matching succeeds,ID=");
    Serial.println(ret);
  }else{
    /*Set fingerprint LED ring to always ON in red*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */0);
    Serial.println("Matching fails");
  }
  Serial.println("-----------------------------");
  delay(1000);
}
