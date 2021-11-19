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
  //connect to WiFi
  WiFi.disconnect();
  delay(500);
  // Access point enabled
  WiFi.softAP(SSID_AP, PASS_AP);
  print("\nAP IP address: ");
  IPAddress ip=WiFi.softAPIP();
  println(ip.toString());
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

void handleWifi() {
  ArduinoOTA.handle();
}
