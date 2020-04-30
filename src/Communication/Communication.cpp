#include <Arduino.h>

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
    s += Serial.read();
  }
  return s;
}

void isConnected() {
  if (abs(millis() - lastTimeoutMillis) > TIME_OUT) {
    connected = false;
    stage = -1;
  }
  if (Serial.available() == 5) {
    connected = checkHeader(readSerial());
    if (connected) {
      stage++;
    } else {
      stage = -1;
    }
  }
}

double getVoltage(double lastVoltage){
if(failedAttempts>2000){
  stage++;
}
if(Serial.available() == 3){
  stage++;
  String volts = readSerial();
  double voltage = volts.toInt() / 100.0;
  return voltage;
}
failedAttempts++;
return lastVoltage;
}






void periodicGetCommands() {
  if (abs(millis() - lastPeriodMillis) > duration) {
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
        Serial.print("#pinggb");
        stage++;
        failedAttempts = 0;
        break;
      case 3: // to do
        break;


      default:
        toggleBlinkingLeds(!connected);
        break;
    }
  }
}
