#include <Arduino.h>

#include "DataClass.h"
#include "LedController/LedController.h"
#define TIME_OUT 5000
uint32_t lastPeriodMillis = millis();
uint32_t duration = 10000;
uint32_t lastTimeoutMillis = millis();
uint32_t failedAttempts = 0;
int stage = 0;
bool connected = true;

void initRFCommunication() { Serial.begin(9600); }
bool checkHeader(String command) {
  String subCommand = command.substring(0, 5);
  return subCommand.equals("#ping");
}

String readSerial() {
  String s = "";
  while (Serial.available() != 0) {
    s += (char)Serial.read();
  }
  return s;
}

void isConnected() {
  if (abs(millis() - lastTimeoutMillis) > TIME_OUT) {
    connected = false;
    stage = -1;
  }
  if (Serial.available() == 5) {
    String s = readSerial();
    connected = checkHeader(s);
    if (connected) {
      toggleBlinkingLeds(false);
      stage++;
    } else {
      stage = -1;
    }
  }
}

double getVoltage(double lastVoltage) {
  if (failedAttempts > 2000) {
    stage++;
  }
  if (Serial.available() == 4) {
    stage++;
    String volts = readSerial();
    volts.remove(3, 1);
    double voltage = volts.toInt() / 100.0;
    return voltage;
  }
  failedAttempts++;
  return lastVoltage;
}
bool getBoolValue(bool lastStatus) {
  if (failedAttempts > 2000) {
    stage++;
    return lastStatus;
  }
  if (Serial.available() == 2) {
    String bools = readSerial();
    stage++;
    if (bools.charAt(0) == '1') {
      return true;
    } else {
      return false;
    }
  }
  failedAttempts++;
  return lastStatus;
}

void periodicGetCommands() {
  if (abs(millis() - lastPeriodMillis) > duration) {
    DataClass* p = DataClass::getInstance();
    DataClass data = *p;
    switch (stage) {
      case 0:
        Serial.print("#ping");
        lastTimeoutMillis = millis();
        stage++;
        break;
      case 1:
        isConnected();
        break;
      case 2:
        data.setConnected(connected);
        Serial.print("#pinggb");
        stage++;
        failedAttempts = 0;
        break;
      case 3:  // to do
        data.setBatteryVoltage(getVoltage(data.getBatteryVoltage()));
        break;
      case 4:
        Serial.print("#pinggs");
        stage++;
        failedAttempts = 0;
        break;
      case 5:
        data.setSolarPanelVoltage(getVoltage(data.getSolarPanelVoltage()));
        break;
      case 6:
        Serial.print("#pinggc");
        stage++;
        failedAttempts = 0;
        break;
      case 7:
        data.setCharging(getBoolValue(data.isCharging()));
        break;
      case 8:
        Serial.print("#pinggw");
        stage++;
        failedAttempts = 0;
        break;
      case 9:
        data.setWaterLevel(getBoolValue(data.isWaterLevelOk()));
        stage++;
        break;
      case 10:
        lastPeriodMillis = millis();
        break;
      default:
        lastPeriodMillis = millis();
        toggleBlinkingLeds(!connected);
        break;
    }
  } else {
    stage = 0;
  }
}
