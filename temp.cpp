/*
   TempSensor: gestione lettura della temperatura da sensore MCP9808
   by Fabrizio Allevi
   fabrizio . allevi @ tiscali . it
*/

#include <Adafruit_MCP9808.h>
#include "main.h"

Adafruit_MCP9808 tempSensor = Adafruit_MCP9808();

void initTemp() {
  if (!tempSensor.begin()) {
    if(DEBUG)
      Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    else
      debugMsg="Couldn't find MCP9808! Check your connections and verify the address is correct.";
  }
  tempSensor.setResolution(1); // sets the resolution 0-0.5°C-30ms | 1-0.25°C-65ms | 2-0.125°C-130ms | 3-0.0625°C-250ms
}

int errTemp=0;
void handleTemp(float &temperature) {
  tempSensor.wake();   // wake up, ready to read!
  float ft=tempSensor.readTempC()+plant.tOffset;
  float delta=abs(temperature-ft);
  if(delta<0.5 || errTemp>2 || temperature==0) {
    temperature=ft;
    errTemp=0;
  }
  else {
    errTemp++;
    debugMsg=String(timeNow.tm_hour)+":"+String(timeNow.tm_min)+" errTemp:"+String(errTemp)+" delta:"+String(delta);
  }
  if (DEBUG) {
    Serial.print("ft: ");
    Serial.print(ft);
    Serial.print(" delta: ");
    Serial.print(delta);
    Serial.print(" temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
  }
  tempSensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
}
