/*
 * schedule service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"

bool force=false;
int minuteForceStart=0;
bool scheduler() {
  bool bRet=false;
  if(plant.bEnable) {
    int minuteNow = timeNow.tm_hour*60+timeNow.tm_min;
    if (DEBUG) {
      Serial.print("minuteNow=");
      Serial.println(minuteNow);
    }
    if(minuteNow>plant.minIni && minuteNow<plant.minFin) {  // temp high during day
      if(ambT<plant.tHigh+plant.tHyst)
        bRet=true;
    }
    if(ambT<plant.tLow+plant.tHyst)  // temp min with plant on
      bRet=true;
    if(force) { // force 30min
      if(minuteForceStart==0) {
        minuteForceStart=minuteNow;
        bRet=true;
      }
      else if(minuteNow>minuteForceStart+30) {
        minuteForceStart=0;
        force=false;
      }
      else {
        bRet=true;
      }
    }
  }
  return bRet;
}
