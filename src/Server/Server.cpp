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

void wifiBegin() {
  EEPROM.get(10, wifi);
  char wifiName[wifi.ssid.length()];
  char wifiPassword[wifi.password.length()];
  for (uint16_t i = 0; i < wifi.ssid.length(); i++){
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
      if (WiFi.status() == 1){
        break;
      }
    }
    if(!WiFi.isConnected()){
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