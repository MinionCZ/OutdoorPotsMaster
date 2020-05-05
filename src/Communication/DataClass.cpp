#include <Arduino.h>
#include "DataClass.h"

DataClass::DataClass() {
  batteryVoltage = 0;
  solarPanelVoltage = 0;
  charging = false;
  connected = false;
  water = false;
  }


void DataClass::setBatteryVoltage(double batteryVoltage) {
  this->batteryVoltage = batteryVoltage;
  }
  void DataClass::setSolarPanelVoltage(double solarPanelCharging) {
    this->solarPanelVoltage = solarPanelCharging;
  }
  void DataClass::setCharging(bool charging) {
    this -> charging = charging;
      }
  void DataClass::setConnected(bool connected) { this->connected = connected; }
  double DataClass::getBatteryVoltage() { return batteryVoltage; }
  double DataClass::getSolarPanelVoltage() { return solarPanelVoltage; }
  bool DataClass::isCharging() {
    return charging;
    }
  bool DataClass::isConnected() { return connected; }
  bool DataClass::isWaterLevelOk() { return water; }
  void DataClass::setWaterLevel(bool waterLevel) { this->water = waterLevel; }