/*
 * data service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"
#include <SPIFFS.h>

stPlant plant = {true, 0.0, 19.0, 20.8, 0.2, 7*60, 22*60, -1};

bool saveData() {
  byte *p=(byte*)&plant;
  File file = SPIFFS.open("/plant", FILE_WRITE);
  if(!file) {
    if(DEBUG)
      Serial.println("An Error has occurred while opening /plant file");
    return false;
  }
  else {
    file.write(p, sizeof(plant));
  }
  file.close();
  return true;
}

bool loadData() {
  byte *p=(byte*)&plant;
  if(!SPIFFS.begin(true)){
    if(DEBUG)
      Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }
  File file = SPIFFS.open("/plant", FILE_READ);
  if(!file)
    saveData(); // no file we need to create it
  else {
    if(file.size()!=sizeof(plant)) {
      if(DEBUG)
        Serial.println("different size between file and struct....now update file!");
      saveData();
    }
    else
      file.read(p, sizeof(plant));
  }
  file.close();
  return true;
}
