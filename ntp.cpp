/*
 * ntp service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include <WiFi.h>
#include <WiFiUdp.h>

#include "main.h"
#include "secret.h"

String ssid[MAX_NETWORKS]={SECRET_SSID_0, SECRET_SSID_1, SECRET_SSID_2, SECRET_SSID_3};
String pass[MAX_NETWORKS]={SECRET_PASS_0, SECRET_PASS_1, SECRET_PASS_2, SECRET_PASS_3};

const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
bool updateNtpTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.inrim.it", "time.nist.gov");
  struct tm timeinfo;
  bool okTime=getLocalTime(&timeinfo);
  if(DEBUG) {
    if(!okTime)
      Serial.println("Failed to obtain time");
    else
      Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  }
  return okTime;
}


int nNet=0;
bool updateNTP() {
  bool bRet=false;
  int retryW=10;
  if(DEBUG) {
    Serial.print("Connecting to ");
    Serial.print(ssid[nNet]);
  }
  WiFi.begin(ssid[nNet].c_str(), pass[nNet].c_str());
  while(!bRet && retryW--) {
    if(DEBUG)
      Serial.print(".");
    delay(1000);
    blink();
    if(WiFi.status()==WL_CONNECTED) {
      if(DEBUG) {
        Serial.println("");
        Serial.println(WiFi.SSID());
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
      }
      int retryN=5;
      while(!bRet && retryN--) {
        bRet=updateNtpTime();
        if(!bRet)
          delay(1000);
        blink();
      }
      WiFi.disconnect();
    }
    if(!bRet) {
      nNet=(nNet+1)%MAX_NETWORKS;
    }
    else {
      while(WiFi.status()!=WL_DISCONNECTED) {
        delay(100);
        if(DEBUG)
          Serial.print(":");
        blink();
      }
    }
  }
  stopBlink();
  return bRet;
}
