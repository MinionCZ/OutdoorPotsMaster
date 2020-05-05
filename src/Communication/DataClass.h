#ifndef DATA_CLASS_H
#define DATA_CLASS_H
#include <Arduino.h>
class DataClass {
 private:
  double batteryVoltage;
  double solarPanelVoltage;
  bool charging;
  bool connected;
  bool water;
 
 public:
  DataClass();
  void setBatteryVoltage(double batteryVoltage);
  double getBatteryVoltage();
  void setSolarPanelVoltage(double solarPanelVoltage);
  double getSolarPanelVoltage();
  void setCharging(bool charging);
  bool isCharging();
  void setConnected(bool connected);
  bool isConnected();
  bool isWaterLevelOk();
  void setWaterLevel(bool waterLevel);
  static DataClass* getInstance();
};

#endif