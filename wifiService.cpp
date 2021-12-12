/*
 * wifi service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "main.h"
#include "secret.h"

void initWifi() {
  print("Connecting to network...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SECRET_SSID_0, SECRET_PASS_0);
  while(WiFi.status()!=WL_CONNECTED) {
    print(".");
    delay(1000);
    blink();
  }
  stopBlink();
  println("");
  print(WiFi.SSID());
  print(" IPaddr=");
  IPAddress ip=WiFi.localIP();
  print(ip.toString());
  print(" RSSI=");
  long rssi = WiFi.RSSI();
  print((int)rssi);
  println("dBm");
  // OTA service
  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        println("Start updating " + type);
      })
      .onEnd([]() {
        println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) println("Receive Failed");
        else if (error == OTA_END_ERROR) println("End Failed");
      });
  ArduinoOTA.begin();
  return;
}

long tWiFi=0;
bool handleWifi() {
  // if WiFi is down, try reconnecting
  if(WiFi.status()!=WL_CONNECTED) {
    long tNow=millis();
    if(tNow-tWiFi>=30000) {
      WiFi.disconnect();
      WiFi.reconnect();
      tWiFi=tNow;
    }
    return false;
  }
  else
    ArduinoOTA.handle();
  return true;
}
