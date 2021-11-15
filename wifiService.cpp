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

String ssidH[2]={SHELLY_SSID_H1, SHELLY_SSID_H2};
String passH[2]={SECRET_PASS_H1, SECRET_PASS_H2};
IPAddress local_IP(192,168,33,200);
IPAddress dns(192,168,33,1);
IPAddress gateway(192,168,33,1);
IPAddress subnet(255,255,255,0);

void initWifi() {
  //connect to WiFi
  WiFi.disconnect();
  delay(500);
  if(plant.nCh==-1) {
    WiFi.softAP(SSID_AP, PASS_AP);
    if(DEBUG) {
      Serial.print("\nAP IP address: ");
      Serial.println(WiFi.softAPIP());
    }
    // OTA service
    ArduinoOTA
        .onStart([]() {
          String type;
          if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
          else // U_SPIFFS
            type = "filesystem";

          // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
          if(DEBUG)
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
          if(DEBUG)
            Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
          if(DEBUG)
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
          if(DEBUG) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed");
          }
        });
    ArduinoOTA.begin();
    return;
  }
  if (!WiFi.config(local_IP, dns, gateway, subnet)) {
    if(DEBUG)
      Serial.println("STA Failed to configure");
    return;
  }
  if(DEBUG) {
    Serial.print("Try connect to ");
    Serial.print(ssidH[plant.nCh].c_str());
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidH[plant.nCh].c_str(), passH[plant.nCh].c_str());
  int retryW=6;
  while(WiFi.status()!=WL_CONNECTED) {
    if(retryW--==0) {
      retryW=6;
      WiFi.disconnect();
      delay(1000);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssidH[plant.nCh].c_str(), passH[plant.nCh].c_str());
      if(DEBUG)
        Serial.println("");
    }
    if(DEBUG)
      Serial.print(".");
    delay(1000);
    blink();
  }
  if(WiFi.status()==WL_CONNECTED){
    if(DEBUG) {
      Serial.print("\nWiFi is connected with IP address: ");
      Serial.println(WiFi.localIP());
    }
  }
  stopBlink();
}

void handleWifi() {
  if(plant.nCh==-1)
    ArduinoOTA.handle();
}
