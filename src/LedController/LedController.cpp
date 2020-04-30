#include <Arduino.h>
#define waterPin D7
#define chargingPin D8
#define batteryPin D6
#define ser D4
#define rclk D3
#define srclk D2
bool waterOn = false;
bool chargingOn = false;
bool batteryOn = false;
bool blinkingOn = false;
bool on = false;
uint32_t ledBlinkDuration = 500;
uint32_t lastMillis = millis();

void clearVoltageLed() {
  digitalWrite(rclk, LOW);
  for (int i = 0; i < 8; i++) {
    digitalWrite(srclk, LOW);
    digitalWrite(ser, LOW);
    digitalWrite(srclk, HIGH);
    delayMicroseconds(1);
  }
  digitalWrite(rclk, HIGH);
}

void initLedController() {
  pinMode(D8, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D2, OUTPUT);
  clearVoltageLed();
}
void toggleWaterLed() {
  waterOn = !waterOn;
  digitalWrite(waterPin, waterOn);
}
void toggleChargingPin() {
  chargingOn = !chargingOn;
  digitalWrite(chargingPin, chargingOn);
}
void toggleBatteryPin() {
  batteryOn = !batteryOn;
  digitalWrite(batteryPin, batteryOn);
}


void showVoltageLeds(int leds) {
  digitalWrite(rclk, LOW);
  for (int i = 0; i < 8; i++) {
    digitalWrite(srclk, LOW);
    digitalWrite(ser, leds > 0);
    digitalWrite(srclk, HIGH);
    leds--;
  }
  digitalWrite(rclk, HIGH);
}
void blinkLeds(){
if (!blinkingOn){
  return;
}
if(abs(millis() - lastMillis)<ledBlinkDuration){
  return;
}
on = !on;
digitalWrite(D8, on);
digitalWrite(D7, on);
digitalWrite(D6, on);
if (on){
  showVoltageLeds(8);
}else{
  clearVoltageLed();
}
lastMillis = millis();
}
void toggleBlinkingLeds(bool status) { blinkingOn = status; }