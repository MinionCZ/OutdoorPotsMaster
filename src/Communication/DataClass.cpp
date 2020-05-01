#include <Arduino.h>
#include "DataClass.h"
DataClass* DataClass::instance = 0;
DataClass::DataClass() {
  batteryVoltage = 0;
  solarPanelVoltage = 0;
  charging = false;
  connected = false;
  water = false;
  }
DataClass* DataClass::getInstance(){
    if(instance == 0){
      instance = new DataClass();
    }
    return instance;
}



  void DataClass::setBatteryVoltage(double batteryVoltage) {
    this->batteryVoltage = batteryVoltage;
  }
  void DataClass::setSolarPanelVoltage(double solarPanelCharging) {
    this->solarPanelVoltage = solarPanelVoltage;
  }
  void DataClass::setCharging(bool charging) { this->charging = charging; }
  void DataClass::setConnected(bool connected) { this->connected = connected; }
  double DataClass::getBatteryVoltage() { return batteryVoltage; }
  double DataClass::getSolarPanelVoltage() { return solarPanelVoltage; }
  bool DataClass::isCharging() { return charging; }
  bool DataClass::isConnected() { return connected; }
  bool DataClass::isWaterLevelOk() { return water; }
  void DataClass::setWaterLevel(bool waterLevel) { this->water = waterLevel; }