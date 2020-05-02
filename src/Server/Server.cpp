#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

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
void sendPing() { server.send(200, "text/plain", "#ping"); }//send ping back
void eraseEEPROM() {
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  server.send(200, "text/plain", "erased");
}
void wifiSetup() {
  WiFi.disconnect();
  IPAddress ip(10, 20, 30, 69);
  IPAddress gateway(10, 0, 0, 138);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet, gateway);
  EEPROM.begin(512);
  eraseEEPROM();
  if (EEPROM.read(0) == 1) {  // written in EEPROM
    EEPROM.get(10, wifi);
    WiFi.begin(wifi.ssid, wifi.password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(2);
    }
    if(!WiFi.isConnected()){
      WiFi.softAPConfig(ip, gateway, subnet);
      WiFi.softAP("Garden", "123456789");
    }
  }
  else {  // not yet written in EEPROM
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP("Garden", "123456789");
  }
}
void connectToWifi() {  // POST

  if (WiFi.isConnected()) {
    WiFi.disconnect();
  } else {
    WiFi.softAPdisconnect();
  }
  wifiSetup();
  if(WiFi.isConnected()){
    server.send(204, "text/plain", "connected");
  }else{
    server.send(418, "text/plain", "not connected");
  }
  
}
void sendSSID(){//GET ssid
  Wifi g;
  EEPROM.get(10, g);
  server.send(200, "text/plain", g.ssid);
}
void sendPassword() {  // GET ssid
  Wifi g;
  EEPROM.get(10, g);
  server.send(200, "text/plain", g.password);
}

void serverInit() {
  server.on("/WIFI", HTTP_POST, saveWifiSettings);
  server.on("/CONNECT", HTTP_POST, connectToWifi);
  server.on("/ERASE", HTTP_POST, eraseEEPROM);
  server.on("/PING", HTTP_GET, sendPing);
  server.on("/SSID", HTTP_GET, sendSSID);
  server.on("/PASSWORD", HTTP_GET, sendPassword);
  server.begin();
}
void serverHandleClient() { server.handleClient(); }