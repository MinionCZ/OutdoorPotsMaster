#include <Arduino.h>
#include "LedController/LedController.h"
#include "Communication/Communication.h"
#include "Communication/DataClass.h"
#include "Server/Server.h"
void setup() {
  // put your setup code here, to run once:
  initLedController();
  initRFCommunication();
  wifiSetup();
  serverInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  blinkLeds();
  periodicGetCommands();
  serverHandleClient();
  delay(1);
}