/*
 * main include file
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include <Arduino.h>
#include <time.h>

#define LED_BUILTIN 1

// exported
//void initAmb();
//void handleAmb(float &temperature, float &humidity);
bool updateNTP();
bool turn(bool state);
bool scheduler();
void APinitWifi();
void APhandleWifi();
void initWifi();
bool handleWifi();
void initWebLog();
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
  int nNet;
};
extern stPlant plant;
extern struct tm timeNow;
extern float ambT;
extern String debugMsg;
extern bool state, reset, force;

void blink();
void stopBlink();
extern int serial;
void print(String m = "");
void print(const char *m);
void print(char *m);
void print(int m);
void print(uint8_t m);
void print(uint16_t m);
void print(uint32_t m);
void print(double m);
void print(float m);
void println(String m = "");
void println(const char *m);
void println(char *m);
void println(int m);
void println(uint8_t m);
void println(uint16_t m);
void println(uint32_t m);
void println(float m);
void println(double m);
