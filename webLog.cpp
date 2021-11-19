/*
 * webLog service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

AsyncWebServer webLog(81);

void recvMsg(uint8_t *data, size_t len){
  print("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  println(d);
  if (d == "ON" && serial==0) {
    serial=-1;
    if(debugMsg!="")
      print(debugMsg.c_str());
  }
  if (d=="OFF" && serial==-1) {
    serial=0;
  }
}

void initWebLog() {
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&webLog);
  WebSerial.msgCallback(recvMsg);
  webLog.begin();
}
