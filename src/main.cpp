#include <Arduino.h>
#include "LedController/LedController.h"
#include "Communication/Communication.h"
void setup() {
  // put your setup code here, to run once:
  initLedController();
  initRFCommunication();
}
int counter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  blinkLeds();
  periodicGetCommands();
  delay(1);
}