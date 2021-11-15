/*
 * relay service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include "main.h"
#include "secret.h"

String serverName = "http://192.168.33.1/relay/0";
String shellyMsg;

bool turn(bool b) {
  bool bRet=false;
  shellyMsg="";
  if(WiFi.status()==WL_CONNECTED){
    HTTPClient http;
    String serverPath = serverName+(b?"?turn=on":"?turn=off");
    http.begin(serverPath.c_str());
    http.setAuthorization(SHELLY_USER, SHELLY_PASS);
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
      if(DEBUG) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      shellyMsg=http.getString();
      if(DEBUG)
        Serial.println(shellyMsg);
      int ini = shellyMsg.indexOf("\"ison\":");
      int fin = shellyMsg.indexOf(",");
      String ss=shellyMsg.substring(ini+7,fin);
      if(DEBUG)
        Serial.println(ss);
      if((b && ss=="true") || (!b && ss=="false"))
        bRet=true;
      else
        bRet=false;
    }
    else {
      if(DEBUG) {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
    }
    http.end();
  }
  else {
    if(DEBUG)
      Serial.println("WiFi Disconnected");
  }
  return bRet;
}
