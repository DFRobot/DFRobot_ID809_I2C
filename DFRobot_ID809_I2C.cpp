/*!
 * @file DFRobot_ID809_I2C.cpp
 * @brief Define the basic structure of DFRobot_ID809_I2C class and the implementation of underlying methods
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [Eddard](eddard.liu@dfrobot.com)
 * @version  V1.1
 * @date  2023-08-04
 * @url https://github.com/cdjq/DFRobot_ID809_I2C
 */

#include <DFRobot_ID809_I2C.h>
#include <Arduino.h>
#include <string.h>
#include <stdio.h>


Stream* dbg = NULL;

#if ((defined ARDUINO_AVR_UNO) || (defined ARDUINO_AVR_NANO) ||(defined NRF5))
SoftwareSerial Serial1(2, 3);  //RX, TX
#endif
DFRobot_ID809::DFRobot_ID809()
{

}
bool DFRobot_ID809::isConnected()
{

  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_TEST_CONNECTION, NULL, 0);

  sendPacket(header);

  free(header);
  if (ISIIC == true)
    delay(50);

  uint8_t ret = responsePayload(buf);

  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    return true;
  } else {
    return false;
  }
}

uint8_t DFRobot_ID809::setDeviceID(uint8_t deviceID)
{
  uint8_t data[5] = { 0 };    //data:1bytes Parameter Type+4bytes Parameter Value
  data[1] = deviceID;
  uint8_t ret = setParam(data);
  return ret;
}

uint8_t DFRobot_ID809::setSecurityLevel(uint8_t securityLevel)
{
  uint8_t data[5] = { 0 };
  data[0] = 1;
  data[1] = securityLevel;
  uint8_t ret = setParam(data);
  return ret;
}

uint8_t DFRobot_ID809::setDuplicationCheck(uint8_t duplicationCheck)
{
  uint8_t data[5] = { 0 };
  data[0] = 2;
  data[1] = duplicationCheck;
  uint8_t ret = setParam(data);
  return ret;
}

uint8_t DFRobot_ID809::setBaudrate(eDeviceBaudrate_t baudrate)
{
  if (ISIIC == true) return ERR_ID809;
  uint8_t data[5] = { 0 };
  data[0] = 3;
  data[1] = baudrate;
  uint8_t ret = setParam(data);
  return ret;
}

uint8_t DFRobot_ID809::setSelfLearn(uint8_t selfLearn)
{
  uint8_t data[5] = { 0 };
  data[0] = 4;
  data[1] = selfLearn;
  uint8_t ret = setParam(data);
  return ret;
}

uint8_t DFRobot_ID809::getDeviceID()
{
  uint8_t data[1];  //data:1byte Parameter Type
  data[0] = 0;
  uint8_t ret = getParam(data);
  return ret;
}

uint8_t DFRobot_ID809::getSecurityLevel()
{
  uint8_t data[1];
  data[0] = 1;
  uint8_t ret = getParam(data);
  return ret;
}

uint8_t DFRobot_ID809::getDuplicationCheck()
{
  uint8_t data[1];
  data[0] = 2;
  uint8_t ret = getParam(data);
  return ret;
}

uint8_t DFRobot_ID809::getBaudrate()
{
  uint8_t data[1];
  data[0] = 3;
  uint8_t ret = getParam(data);
  return ret;
}

uint8_t DFRobot_ID809::getSelfLearn()
{
  uint8_t data[1];
  data[0] = 4;
  uint8_t ret = getParam(data);
  return ret;
}

String DFRobot_ID809::getDeviceInfo()
{
  char* data;
  uint8_t result;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_DEVICE_INFO, NULL, 0);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(50);
  result = responsePayload(buf);
  LDBG("result=");
  LDBG(result);
  if (result != ERR_SUCCESS) {
    return "";
  }
  uint16_t dataLen = buf[0] + (buf[1] << 8) + 1;
  //Serial.print("dataLen=");
//Serial.println(dataLen);
  if ((data = (char*)malloc(dataLen + 10)) == NULL) {
    LDBG("no memory!!!\r\n");
    while (1);
  }
  data[dataLen] = 0;
  result = responsePayload(data);

  String ret = String(data);
  free(data);
  return ret;
}

uint8_t DFRobot_ID809::setModuleSN(const char* SN)
{
  char data[2];
  data[0] = MODULE_SN_SIZE;
  if (strlen(SN) > MODULE_SN_SIZE) {
    LDBG("The serial number exceeds 15 characters");
    return ERR_ID809;
  }
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_SET_MODULE_SN, data, 2);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(100);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret != ERR_SUCCESS) {
    return ERR_ID809;
  }
  header = pack(DATA_TYPE, CMD_SET_MODULE_SN, SN, MODULE_SN_SIZE);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(240);
  ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    ret = buf[0];
  }
  return ret;
}

String DFRobot_ID809::getModuleSN()
{
  char* data;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GET_MODULE_SN, NULL, 0);
  sendPacket(header);
  if (ISIIC == true)
    delay(100);
  free(header);
  uint8_t result = responsePayload(buf);
  LDBG("result=");
  LDBG(result);
  if (result != ERR_SUCCESS) {
    return "";
  }
  uint16_t dataLen = buf[0] + (buf[1] << 8) + 1;
  if ((data = (char*)malloc(dataLen + 1)) == NULL) {
    LDBG("no memory!!!\r\n");
    while (1);
  }
  data[dataLen] = 0;
  result = responsePayload(data);

  LDBG("result=");
  LDBG(result);

  String ret = String(data);

  free(data);

  return ret;
}
/*wang*/
uint8_t DFRobot_ID809::ctrlLED(eLEDMode_t mode, eLEDColor_t color, uint8_t blinkCount)
{
  char data[4] = { 0 };
  if (FINGERPRINT_CAPACITY == 80) {
    data[0] = mode;
    data[2] = data[1] = color;
    data[3] = blinkCount;
  } else {
    if (mode == 1) {
      data[0] = 2;
    } else if (mode == 2) {
      data[0] = 4;
    } else if (mode == 3) {
      data[0] = 1;
    } else if (mode == 4) {
      data[0] = 0;
    } else if (mode == 5) {
      data[0] = 3;
    }
    if (color == eLEDGreen) {
      data[2] = data[1] = 0x84;
    } else if (color == eLEDRed) {
      data[2] = data[1] = 0x82;
    } else if (color == eLEDYellow) {
      data[2] = data[1] = 0x86;
    } else if (color == eLEDBlue) {
      data[2] = data[1] = 0x81;
    } else if (color == eLEDCyan) {
      data[2] = data[1] = 0x85;
    } else if (color == eLEDMagenta) {
      data[2] = data[1] = 0x83;
    } else {
      data[2] = data[1] = 0x87;
    }
  }
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_SLED_CTRL, data, 4);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(50);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  return ret;
}
/*wang*/
uint8_t DFRobot_ID809::detectFinger()
{
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_FINGER_DETECT, NULL, 0);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(240);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    ret = buf[0];
  }
  return ret;
}
/*wang*/
uint8_t DFRobot_ID809::getEmptyID()
{
  char data[4] = { 0 };
  data[0] = 1;
  data[2] = FINGERPRINT_CAPACITY;     //80 fingerprints at most, default to full range
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GET_EMPTY_ID, data, 4);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(100);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    ret = buf[0];
  }
  return ret;
}

uint8_t DFRobot_ID809::getStatusID(uint8_t ID)
{
  char data[2] = { 0 };
  data[0] = ID;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GET_STATUS, data, 2);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(50);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    ret = buf[0];
  }
  return ret;
}

uint8_t DFRobot_ID809::getEnrollCount()
{
  char data[4] = { 0 };
  data[0] = 1;
  data[2] = FINGERPRINT_CAPACITY;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GET_ENROLL_COUNT, data, 4);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(80);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    ret = buf[0];
  }
  return ret;
}

#define  getID(A, V)  (A[0 + V/8] & (0x01 << (V & 0x07)))
uint8_t DFRobot_ID809::getEnrolledIDList(uint8_t* list)
{
  char* data;
  uint8_t i = 0;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GET_ENROLLED_ID_LIST, NULL, 0);
  sendPacket(header);

  free(header);
  if (ISIIC == true)
    delay(120);

  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  uint16_t dataLen = buf[0] + (buf[1] << 8);
  if ((data = (char*)malloc(dataLen + 10)) == NULL) {
    LDBG("no memory!!!\r\n");
    while (1);
  }
  ret = responsePayload(data);
  LDBG("ret=");
  LDBG(ret);
  if (ret != ERR_SUCCESS) {
    ret = ERR_ID809;
  } else {
    for (uint16_t j = 0; j < (dataLen * 8); j++) {
      if (getID(data, j) != 0) {
        list[i] = j;
        i++;
      }
    }
  }
  free(data);
  return ret;
}

uint8_t DFRobot_ID809::storeFingerprint(uint8_t ID)
{
  char data[4] = { 0 };
  uint8_t ret = merge();
  LDBG("ret=");
  LDBG(ret);
  if (ret != ERR_SUCCESS) {
    return ERR_ID809;
  }
  _number = 0;
  data[0] = ID;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_STORE_CHAR, data, 4);
  sendPacket(header);

  free(header);
  if (ISIIC == true)
    delay(360);
  ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  return ret;

}

uint8_t DFRobot_ID809::delFingerprint(uint8_t ID)
{
  char data[4] = { 0 };
  if (ID == DELALL) {
    data[0] = 1;
    data[2] = FINGERPRINT_CAPACITY;
  } else {
    data[0] = data[2] = ID;
  }
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_DEL_CHAR, data, 4);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(360);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  return ret;
}

uint8_t DFRobot_ID809::search()
{
  if (_state == 1) {
    char data[6] = { 0 };
    data[2] = 1;
    data[4] = FINGERPRINT_CAPACITY;
    _number = 0;
    pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_SEARCH, data, 6);
    sendPacket(header);
    free(header);
    if (ISIIC == true)
      delay(360);
    uint8_t ret = responsePayload(buf);
    LDBG("ret=");
    LDBG(ret);
    if (ret == ERR_SUCCESS) {
      ret = buf[0];
    } else {
      ret = 0;
    }
    return ret;
  }
  return 0;
}

uint8_t DFRobot_ID809::verify(uint8_t ID)
{
  if (_state == 1) {
    char data[4] = { 0 };
    data[0] = ID;
    _number = 0;
    pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_VERIFY, data, 4);
    sendPacket(header);
    free(header);
    if (ISIIC == true)
      delay(50);
    uint8_t ret = responsePayload(buf);
    LDBG("ret=");
    LDBG(ret);
    if (ret == ERR_SUCCESS) {
      ret = buf[0];
    } else {
      ret = 0;
    }
    return ret;
  }
  return 0;
}

uint8_t DFRobot_ID809::match(uint8_t RamBufferID0, uint8_t RamBufferID1)
{
  char data[4] = { 0 };
  data[0] = RamBufferID0;
  data[2] = RamBufferID1;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_MATCH, data, 4);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(100);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    ret = buf[0];
  } else {
    ret = 0;
  }
  return ret;
}

uint8_t DFRobot_ID809::getBrokenQuantity()
{
  char data[4] = { 0 };
  data[0] = 1;
  data[2] = FINGERPRINT_CAPACITY;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GET_BROKEN_ID, data, 4);

  sendPacket(header);

  free(header);
  if (ISIIC == true)
    delay(50);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    ret = buf[0];
  }
  return ret;
}

uint8_t DFRobot_ID809::getBrokenID()
{
  char data[4] = { 0 };
  data[0] = 1;
  data[2] = FINGERPRINT_CAPACITY;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GET_BROKEN_ID, data, 4);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(50);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    ret = buf[2];
  }
  return ret;
}

uint8_t DFRobot_ID809::loadFingerprint(uint8_t ID, uint8_t RamBufferID)
{
  char data[4] = { 0 };
  data[0] = ID;
  data[2] = RamBufferID;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_LOAD_CHAR, data, 4);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(50);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  return ret;
}

uint8_t DFRobot_ID809::enterSleepState()
{
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_ENTER_STANDBY_STATE, NULL, 0);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(50);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  return ret;
}
/*wang*/
uint8_t DFRobot_ID809::setParam(uint8_t* data)
{
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_SET_PARAM, (const char*)data, 5);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(240);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  return ret;
}

uint8_t DFRobot_ID809::getParam(uint8_t* data)
{
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GET_PARAM, (const char*)data, 1);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(50);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  if (ret == ERR_SUCCESS) {
    ret = buf[0];
  }
  return ret;
}

uint8_t DFRobot_ID809::getImage()
{
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GET_IMAGE, NULL, 0);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(360);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  return ret;
}

uint8_t DFRobot_ID809::collectionFingerprint(uint16_t timeout)   //Collect fingerprint
{
  uint16_t i = 0;
  uint8_t ret;
  if (_number > 2) {
    _error = eErrorGatherOut;
    LDBG("Exceed upper limit of acquisition times ");
    return ERR_ID809;
  }
  while (!detectFinger()) {
    delay(10);
    if (++i > timeout * 10) {
      _error = eErrorTimeOut;
      LDBG("Acquisition timeout ");
      LDBG("ret=");
      LDBG(ret);
      _state = 0;
      return ERR_ID809;
    }
  }

  ret = getImage();
  LDBG("ret=");
  LDBG(ret);
  if (ret != ERR_SUCCESS) {
    _state = 0;
    return ERR_ID809;
  }
  ret = generate(_number);
  LDBG("ret=");
  LDBG(ret);
  if (ret != ERR_SUCCESS) {
    _state = 0;
    return ERR_ID809;
  }
  _number++;
  _state = 1;
  return ret;
}
/*wang*/
uint8_t DFRobot_ID809::generate(uint8_t RamBufferID)
{
  char data[2] = { 0 };
  data[0] = RamBufferID;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_GENERATE, (const char*)data, 2);

  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(360);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  return ret;
}
/*wang*/
uint8_t DFRobot_ID809::merge()
{
  char data[3] = { 0 };
  data[2] = _number;
  pCmdPacketHeader_t header = pack(CMD_TYPE, CMD_MERGE, data, 3);
  sendPacket(header);
  free(header);
  if (ISIIC == true)
    delay(360);
  uint8_t ret = responsePayload(buf);
  LDBG("ret=");
  LDBG(ret);
  return ret;
}

pCmdPacketHeader_t DFRobot_ID809::pack(uint8_t type, uint16_t cmd, const char* payload, uint16_t len)
{
  pCmdPacketHeader_t header;
  uint16_t cks = 0;
  uint16_t dataLen;
  if (type == CMD_TYPE) {   //Structure of command packet, fixed 26 bytes：10(frame header)+14(data)+2(CKS)
    if ((header = (pCmdPacketHeader_t)malloc(sizeof(sCmdPacketHeader_t) + 16 + 2)) == NULL) {
      return NULL;
    }
    header->PREFIX = CMD_PREFIX_CODE;
    for (int i = 0; i < 16; i++) {
      header->payload[i] = 0;
    }
    dataLen = 16;   //Length of data to be replicated
  } else {                  //Structure of command data packet, unfixed length：10(frame header)+LEN(data)+2(CKS)
    if ((header = (pCmdPacketHeader_t)malloc(sizeof(sCmdPacketHeader_t) + len + 2)) == NULL) {
      return NULL;
    }
    header->PREFIX = CMD_DATA_PREFIX_CODE;
    dataLen = len;   //Length of data to be replicated
  }
  header->SID = 0;
  header->DID = 0;
  header->CMD = cmd;
  header->LEN = len;
  if (len) {
    memcpy(header->payload, payload, len);
  }
  cks = getCmdCKS(header);
  memcpy(&header->payload[dataLen], &cks, 2);
  _PacketSize = sizeof(sCmdPacketHeader_t) + dataLen + 2;
  return header;
}

uint8_t DFRobot_ID809::responsePayload(void* buf)
{
  sRcmPacketHeader_t header;
  uint16_t dataLen, dataCount, cks;
  uint8_t ret;
  int16_t type;
  type = readPrefix(&header);

  if (type == 1) {
    LDBG("--recv timeout---");
    _error = eErrorRecvTimeout;
    return ERR_ID809;
  }
  pRcmPacketHeader_t packet;
  if (type == RCM_TYPE) {   //Structure of response packet, fixed 26 bytes: 10(frame header)+14(data)+2(CKS)
    packet = (pRcmPacketHeader_t)malloc(sizeof(sRcmPacketHeader_t) + 14 + 2);
    dataLen = 14 + 2;      //Length of data+CKS
    if (packet == NULL) {
      LDBG("");
      while (1);
    }
  } else {                  //Structure of response data packet, unfixed length: 10(frame header)+(LEN-2)(data)+2(CKS)
    packet = (pRcmPacketHeader_t)malloc(sizeof(sRcmPacketHeader_t) + header.LEN);
    dataLen = header.LEN;  //Length of data+CKS
    if (packet == NULL) {
      LDBG("");
      while (1);
    }
  }

  memcpy(packet, &header, 10);
  //Serial.println(dataLen);
  dataCount = readN(packet->payload, dataLen);
  cks = packet->payload[dataLen - 2] + (packet->payload[dataLen - 1] << 8);
  ret = (header.RET & 0xFF);
  _error = (eError_t)ret;
  if (ret != ERR_SUCCESS) {
    ret = ERR_ID809;
  } else if (dataLen != dataCount) {
    LDBG("--recvRspPacket length error---");
    _error = eErrorRecvLength;
    ret = ERR_ID809;
  } else if (getRcmCKS(packet) != cks) {
    LDBG("--recvRspPacket cks error---");
    _error = eErrorRecvCks;
    ret = ERR_ID809;
  } else {
    LDBG("--recvRspPacket OK---");
    memcpy(buf, packet->payload, dataLen);
  }
  free(packet);
  packet = NULL;
  return ret;
}

uint16_t DFRobot_ID809::readPrefix(pRcmPacketHeader_t header)
{
  uint8_t ch, ret;
  typedef enum {
    RECV_HEADER_INIT,
    RECV_HEADER_AA,
    RECV_HEADER_A5,
    RECV_HEADER_OK
  } eRecvHeaderState;
  eRecvHeaderState state = RECV_HEADER_INIT;

  long curr = millis();

  while (state != RECV_HEADER_OK) {  //Can judge the received command packet and command data packet prefix at the same time
    if (readN(&ch, 1) != 1) {
      ret = 1;
      return ret;
    }

    if ((ch == 0xAA) && (state == RECV_HEADER_INIT)) {
      state = RECV_HEADER_AA;
      continue;
    } else if ((ch == 0xA5) && (state == RECV_HEADER_INIT)) {
      state = RECV_HEADER_A5;
      continue;
    } else if ((ch == 0x55) && (state == RECV_HEADER_AA)) {
      state = RECV_HEADER_OK;
      ret = RCM_TYPE;
      continue;
    } else if ((ch == 0x5A) && (state == RECV_HEADER_A5)) {
      state = RECV_HEADER_OK;
      ret = DATA_TYPE;
      continue;
    } else {
      state = RECV_HEADER_INIT;
      if (ch == 0xAA) {
        state = RECV_HEADER_AA;
      } else if (ch == 0xA5) {
        state = RECV_HEADER_A5;
      }
      if (millis() - curr > 2000) {
        LDBG("----------!!!!!!!!!recv timeout----------");
        return 1;
      }
    }
  }
  if (ret == RCM_TYPE) {
    header->PREFIX = RCM_PREFIX_CODE;
  } else if (ret == DATA_TYPE) {
    header->PREFIX = RCM_DATA_PREFIX_CODE;
  }
  readN(&header->SID, 1);
  readN(&header->DID, 1);
  readN(&header->RCM, 2);
  readN(&header->LEN, 2);
  readN(&header->RET, 2);
  return ret;
}



uint16_t DFRobot_ID809::getCmdCKS(pCmdPacketHeader_t packet)
{
  uint16_t cks = 0xFF;
  cks += packet->SID;
  cks += packet->DID;
  cks += packet->CMD & 0xFF;
  cks += packet->CMD >> 8;
  cks += packet->LEN & 0xFF;
  cks += packet->LEN >> 8;
  if (packet->LEN > 0) {
    uint8_t* p = packet->payload;
    for (uint8_t i = 0; i < packet->LEN; i++) {
      cks += p[i];
    }
  }
  return cks & 0xFFFF;
}

uint16_t DFRobot_ID809::getRcmCKS(pRcmPacketHeader_t packet)
{
  uint16_t cks = 0xFF;
  cks += packet->SID;
  cks += packet->DID;
  cks += packet->RCM & 0xFF;
  cks += packet->RCM >> 8;
  cks += packet->LEN & 0xFF;
  cks += packet->LEN >> 8;
  cks += packet->RET & 0xFF;
  cks += packet->RET >> 8;
  if (packet->LEN > 0) {
    uint8_t* p = packet->payload;
    for (uint8_t i = 0; i < packet->LEN - 2; i++) {
      cks += p[i];
    }
  }
  return cks & 0xFFFF;
}

const DFRobot_ID809::sErrorDescription_t DFRobot_ID809::errorDescriptionsTable[]PROGMEM = {
  {eErrorSuccess, "Command processed successfully"},
  {eErrorFail, "Command processing failed"},
  {eErrorVerify, "1:1 comparison failed"},
  {eErrorIdentify, "Comparison with all fingerprints failed"},
  {eErrorTmplEmpty, "No fingerprint in designated ID"},
  {eErrorTmplNotEmpty, "Designated ID has fingerprint"},
  {eErrorAllTmplEmpty, "Module unregistered fingerprint"},
  {eErrorEmptyIDNoexist, "No registerable ID here"},
  {eErrorBrokenIDNoexist, "No broken fingerprint"},
  {eErrorInvalidTmplData, "Invalid desingated fingerprint data"},
  {eErrorDuplicationID, "The fingerprint has been registered"},
  {eErrorBadQuality, "Poor quality fingerprint image"},
  {eErrorMergeFail, "Fingerprint synthesis failed"},
  {eErrorNotAuthorized, "Communication password not authorized"},
  {eErrorMemory, "External Flash burning error"},
  {eErrorInvalidTmplNo, "Invalid designated ID"},
  {eErrorInvalidParam, "Incorrect parameter"},
  {eErrorTimeOut, "Acquisition timeout"},
  {eErrorGenCount, "Invalid number of fingerprint synthesis"},
  {eErrorInvalidBufferID, "Incorrect Buffer ID value"},
  {eErrorFPNotDetected, "No fingerprint input into fingerprint reader"},
  {eErrorFPCancel, "Command cancelled"},
  {eErrorRecvLength, "Wrong data length"},
  {eErrorRecvCks, "Wrong data check code"},
  {eErrorGatherOut, "Exceed upper limit of acquisition times"},
  {eErrorRecvTimeout,"Data reading timeout"}
};

String DFRobot_ID809::getErrorDescription()
{
  for (uint32_t i = 0; i < sizeof(errorDescriptionsTable) / sizeof(errorDescriptionsTable[0]); i++) {
    if (_error == errorDescriptionsTable[i].error) {
      return errorDescriptionsTable[i].description;
    }
  }
  return "";
}


DFRobot_ID809_I2C::DFRobot_ID809_I2C(TwoWire* pWire, uint8_t address)
{
  _deviceAddr = address;
  _pWire = pWire;
  //ISIIC = true;
}

bool DFRobot_ID809_I2C::begin()
{
  _pWire->begin();


  String str = getDeviceInfo();
  //Serial.println(str[str.length()-1]);
  if (str[str.length() - 1] == '4') {

    FINGERPRINT_CAPACITY = 80;

    //Serial.println(str[str.length()-1]);
  } else if (str[str.length() - 1] == '3') {
    //Serial.println(str[str.length()-1]);
    FINGERPRINT_CAPACITY = 200;

  }
  return true;
}

void DFRobot_ID809_I2C::sendPacket(pCmdPacketHeader_t pBuf)
{
  if (pBuf == NULL) {
    LDBG("pBuf ERROR!! : null pointer");
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;

  _pWire->requestFrom(_deviceAddr, (uint8_t)1);

  if (0xee != _pWire->read()) {
    _pWire->requestFrom(_deviceAddr, (uint8_t)1);

    while (0xee != _pWire->read()) {
      _pWire->requestFrom(_deviceAddr, (uint8_t)1);
#if defined(ESP8266) 
      yield();
#endif
    }
    _pWire->requestFrom(_deviceAddr, (uint8_t)1);
  }

  _pWire->beginTransmission(_deviceAddr);
  // Serial.println("tx->");
  for (uint16_t i = 0; i < _PacketSize; i++) {
    _pWire->write(_pBuf[i]);
    //Serial.print(_pBuf[i],HEX);
  //	Serial.print(" ");
  }
  _pWire->endTransmission();
}

size_t DFRobot_ID809_I2C::readN(void* pBuf, size_t size)
{
  uint8_t len = size;
  if (pBuf == NULL) {
    LDBG("pBuf ERROR!! : null pointer");
  }

  uint8_t* _pBuf = (uint8_t*)pBuf;

 #if !CONFIG_IDF_TARGET_ESP32C3 && !CONFIG_IDF_TARGET_ESP32S3
   _pWire->beginTransmission(_deviceAddr);
 #endif
  while (size > 32) {

    _pWire->requestFrom(_deviceAddr, (uint8_t)32);
    while (_pWire->available()) {
      *_pBuf = _pWire->read();   // Use read() to receive and put into buf
      _pBuf++;
    }
    // for (uint16_t i = 0; i < 32; i++) {
    //   _pWire->requestFrom(_deviceAddr, 1);
    //   _pBuf[i + len - size] = _pWire->read();
    // }
    size -= 32;
  }
  // Serial.println("rx->");

   for (uint16_t i = 0; i < size; i++) {
     //Serial.println("requestFrom->");
     _pWire->requestFrom(_deviceAddr, (uint8_t)1);
     //  Serial.println("dowm->");
     _pBuf[i] = _pWire->read();
     //Serial.print(_pBuf[i + len - size],HEX);
     //	Serial.print(" ");
   }
  //_pWire->requestFrom(_deviceAddr, (uint8_t)size);   // Master device requests size bytes from slave device, which can be accepted by master device with read() or available()
 // while (_pWire->available()) {
  //  *_pBuf = _pWire->read();   // Use read() to receive and put into buf
   // _pBuf++;
  //}
  // delay(10);

  #if !CONFIG_IDF_TARGET_ESP32C3 && !CONFIG_IDF_TARGET_ESP32S3
   if( _pWire->endTransmission() != 0) {
     return 0;
    }
 # endif

  return len;
}
