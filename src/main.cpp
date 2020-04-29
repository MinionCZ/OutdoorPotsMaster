#include <Arduino.h>
#include "LedController/LedController.h"
void setup() {
  // put your setup code here, to run once:
  initLedController();
}
int counter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  //clearVoltageLed();
  if(counter == 9){
    counter = 1;
  }

  showVoltageLeds(counter);
  counter++;
  delay(1000);
}