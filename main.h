/*
 * main include file
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

// defines
//#define DEBUG true
#define DEBUG false

#include <Arduino.h>
#include <time.h>

// exported
//void initAmb();
//void handleAmb(float &temperature, float &humidity);
bool updateNTP();
bool turn(bool state);
bool scheduler();
void initWifi();
void handleWifi();
void initWeb();
void handleWeb();
bool loadData();
bool saveData();
void initTemp();
void handleTemp(float &temperature);


// data store
struct stPlant {
  bool bEnable;
  float tOffset;
  float tLow;
  float tHigh;
  float tHyst;
  int minIni;
  int minFin;
  int nCh;
};
extern stPlant plant;
extern struct tm timeNow;
extern float ambT, ambH;
extern String shellyMsg, debugMsg;
extern bool state, reset, force;
void blink();
void stopBlink();
