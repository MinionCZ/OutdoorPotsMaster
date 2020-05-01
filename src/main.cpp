#include <Arduino.h>
#include "LedController/LedController.h"
#include "Communication/Communication.h"
#include "Communication/DataClass.h"
DataClass* d;
void setup() {
  // put your setup code here, to run once:
  initLedController();
  initRFCommunication();
  d = DataClass::getInstance();
}

void loop() {
  // put your main code here, to run repeatedly:
  blinkLeds();
  periodicGetCommands();
  delay(1);
}