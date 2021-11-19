/*
 * ambience app
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"

float ambT=0.0f;
bool state=false, reset=false, firstLoop=true;
struct tm timeNow;
long tLast=0;


void setup() {
  if(serial==1) {
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
      delay(1000); // wait for serial port to connect. Needed for native USB port only
    }
  }
  else
    pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
  println("setup start!");
  loadData();
  initTemp();
  while(!updateNTP()) {
    delay(500);
  }
  initWifi();
  initWebLog();
  initWeb();
}

void loop() {
  long tNow=millis();
  if(tNow>tLast+5000 && reset)
    ESP.restart();
  if(tNow>tLast+10000) {
    handleTemp(ambT);
    getLocalTime(&timeNow);
    bool newState=scheduler();
    if(newState!=state || firstLoop) {
      state=newState;
      if(plant.nCh!=-1)
        turn(state);
      println(state?"Heater is ON":"Heater is OFF");
      if(serial!=1) {
        if(state)
          digitalWrite(LED_BUILTIN, LOW);
        else
          digitalWrite(LED_BUILTIN, HIGH);
      }
      if(firstLoop && serial==2)
        serial=0;
      firstLoop=false;
    }
    tLast=tNow;
  }
  handleWifi();
  handleWeb();
}
