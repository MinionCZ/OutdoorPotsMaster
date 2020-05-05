#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "Communication/Communication.h"
#include "Communication/DataClass.h"
#include "EEPROM.h"
#include "ESP8266WiFi.h"

ESP8266WebServer server(80);

WiFiClient client;
struct Wifi {
  String ssid;
  String password;
} wifi;

void saveWifiSettings() {  // POST
  wifi.ssid = server.arg("ssid");
  wifi.password = server.arg("password");
  EEPROM.put(10, wifi);
  EEPROM.write(0, 1);
  EEPROM.commit();
  server.send(204, "text/plain", "set");
}
void sendPing() { server.send(200, "text/plain", "#ping"); }  // send ping back
void eraseEEPROM() {
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  server.send(200, "text/plain", "erased");
}

void wifiBegin() {
  EEPROM.get(10, wifi);
  char wifiName[wifi.ssid.length()];
  char wifiPassword[wifi.password.length()];
  for (uint16_t i = 0; i < wifi.ssid.length(); i++) {
    wifiName[i] = wifi.ssid.charAt(i);
  }
  for (uint16_t i = 0; i < wifi.password.length(); i++) {
    wifiPassword[i] = wifi.password.charAt(i);
  }
  WiFi.begin(wifiName, wifiPassword);
}

void wifiSetup() {
  WiFi.disconnect();
  WiFi.softAPdisconnect();
  IPAddress ip(10, 0, 0, 47);
  IPAddress gateway(10, 0, 0, 138);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet, gateway);
  EEPROM.begin(512);
  if (EEPROM.read(0) == 1) {  // written in EEPROM
    WiFi.mode(WIFI_STA);
    wifiBegin();
    while (WiFi.status() != WL_CONNECTED) {
      delay(2);
      if (WiFi.status() == 1) {
        break;
      }
    }
    if (!WiFi.isConnected()) {
      WiFi.softAPConfig(ip, gateway, subnet);
      WiFi.softAP("Garden", "123456789");
      WiFi.mode(WIFI_AP);
    }
  } else {  // not yet written in EEPROM
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP("Garden", "123456789");
  }
}
void connectToWifi() {  // POST
  wifiSetup();
  if (WiFi.isConnected()) {
    server.send(204, "text/plain", "connected");
  } else {
    server.send(418, "text/plain", "not connected");
  }
}
void sendSSID() {  // GET ssid
  Wifi g;
  EEPROM.get(10, g);
  server.send(200, "text/plain", g.ssid);
}
void sendPassword() {  // GET password
  Wifi g;
  EEPROM.get(10, g);
  server.send(200, "text/plain", g.password);
}
void sendChargingState() {  // GET
  DataClass data = getData();
  server.send(200, "text/plain", data.isCharging() ? "1" : "0");
}
void sendBatteryVoltage() {  // GET
  DataClass data = getData();
  String s = "";
  s += data.getBatteryVoltage();
  server.send(200, "text/plain", s);
}
void sendSolarPanelVoltage() {  // GET
  DataClass data = getData();
  String s = "";
  s += data.getSolarPanelVoltage();
  server.send(200, "text/plain", s);
}
void sendIsConnected() {  // GET
  DataClass data = getData();
  server.send(200, "text/plain", data.isConnected() ? "1" : "0");
}
void sendIsWaterOk() {  // GET
  DataClass data = getData();
  server.send(200, "text/plain", data.isWaterLevelOk() ? "1" : "0");
}
void setLedOn() {  // POST
  String s = "#pingsl";
  s += server.arg("led");
  Serial.print(s);
  server.send(204, "text/plain", "ok");
}

void setTime() {  // POST
  String time = server.arg("hour");
  time += ";";
  time += server.arg("min");
  time += ";";
  time += server.arg("sec");
  time += ";";
  Serial.print("#pingst" + time);
  server.send(204, "text/plain", "ok");
}
void getTime() {// GET
  Serial.print("#pinggt");
  delay(10);
  String time = "";
  while (Serial.available() != 0) {
    time += (char)Serial.read();
  }
  server.send(200, "text/plain", time);
}
void setWatering(){//POST
  String command = "#pingswa";
  command += server.arg("duration");
  command += ";";
  command += server.arg("hour");
  command += ";";
  command += server.arg("minute");
  command += ";";
  command += server.arg("morning");
  Serial.print(command);
  server.send(200, "text/plain", "ok");
}





void serverInit() {  // wifi commands
  server.on("/WIFI", HTTP_POST, saveWifiSettings);
  server.on("/CONNECT", HTTP_POST, connectToWifi);
  server.on("/ERASE", HTTP_POST, eraseEEPROM);
  server.on("/PING", HTTP_GET, sendPing);
  server.on("/SSID", HTTP_GET, sendSSID);
  server.on("/PASSWORD", HTTP_GET, sendPassword);
  server.on("/CHARGING", HTTP_GET, sendChargingState);
  server.on("/BATTERY", HTTP_GET, sendBatteryVoltage);
  server.on("/SOLAR", HTTP_GET, sendSolarPanelVoltage);
  server.on("/CONNECTED", HTTP_GET, sendIsConnected);
  server.on("/WATER", HTTP_GET, sendIsWaterOk);
  server.on("/LED", HTTP_POST, setLedOn);
  server.on("/GTIME", HTTP_GET, getTime);
  server.on("/STIME", HTTP_POST, setTime);
  server.on("/AUTOWATERING", HTTP_POST, setWatering);
  server.begin();
}
void serverHandleClient() { server.handleClient(); }