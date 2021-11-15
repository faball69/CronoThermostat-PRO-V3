/*
 * ambience app
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"
#define LED_BUILTIN 1

float ambT=0.0f, ambH=0.0f;
bool state=false, reset=false, firstLoop=true;
struct tm timeNow;
long tLast=0;
String debugMsg="";


void setup() {
  if(DEBUG) {
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
      delay(1000); // wait for serial port to connect. Needed for native USB port only
    }
  }
  else
    pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
  if(DEBUG)
    Serial.println("INIT");
  loadData();
  //initAmb();
  initTemp();
  while(!updateNTP()) {
    delay(1000);
  }
  initWifi();
  initWeb();
}

void loop() {
  long tNow=millis();
  if(tNow>tLast+5000 && reset)
    ESP.restart();
  if(tNow>tLast+10000/*60000*/) { // one per minute low power consumption
    //handleAmb(ambT, ambH);
    handleTemp(ambT);
    getLocalTime(&timeNow);
    bool newState=scheduler();
    if(newState!=state || firstLoop) {
      state=newState;
      if(plant.nCh!=-1 )
        turn(state);
      if(DEBUG)
        Serial.println(state?"Heater is ON":"Heater is OFF");
      if(state)
        digitalWrite(LED_BUILTIN, LOW);
      else
        digitalWrite(LED_BUILTIN, HIGH);
      firstLoop=false;
    }
    tLast=tNow;
  }
  handleWifi();
  handleWeb();
}

bool toggle=false;
void blink() {
  if(!DEBUG) {
    if(toggle)
      digitalWrite(LED_BUILTIN, HIGH);
    else
      digitalWrite(LED_BUILTIN, LOW);
    toggle=!toggle;
  }
}
void stopBlink() {
  if(!DEBUG)
    digitalWrite(LED_BUILTIN, HIGH);
}
