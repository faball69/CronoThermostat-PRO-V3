/*
 * relay service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include "main.h"
#include "secret.h"

String serverName;
bool turn(bool b) {
  bool bRet=false;
  if(plant.nCh!=-1) {
    serverName=(plant.nCh==1?HEATER2:HEATER1);
    HTTPClient http;
    String serverPath = serverName+(b?"?turn=on":"?turn=off");
    http.begin(serverPath.c_str());
    http.setAuthorization(SHELLY_USER, SHELLY_PASS);
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
      print("HTTP Response code: ");
      println(httpResponseCode);
      String shellyMsg=http.getString();
      println(shellyMsg);
      int ini = shellyMsg.indexOf("\"ison\":");
      int fin = shellyMsg.indexOf(",");
      String ss=shellyMsg.substring(ini+7,fin);
      println(ss);
      if((b && ss=="true") || (!b && ss=="false"))
        bRet=true;
      else
        bRet=false;
    }
    else {
      print("Error code: ");
      println(httpResponseCode);
    }
    http.end();
  }
  return bRet;
}
