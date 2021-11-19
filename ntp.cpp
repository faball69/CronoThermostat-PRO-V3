/*
 * ntp service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include <WiFi.h>
#include <WiFiUdp.h>

#include "main.h"
#include "secret.h"

String ssid[MAX_NETWORKS]={SECRET_SSID_0, SECRET_SSID_1, SECRET_SSID_2};
String pass[MAX_NETWORKS]={SECRET_PASS_0, SECRET_PASS_1, SECRET_PASS_2};

/*struct tm {
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
  ...
}*/
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
bool updateNtpTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.inrim.it", "time.nist.gov");
  struct tm timeinfo;
  bool okTime=getLocalTime(&timeinfo);
  if(!okTime)
    println("Failed to obtain time");
  else {
    print(timeinfo.tm_year+1900); print("-");
    print(timeinfo.tm_mon); print("-");
    print(timeinfo.tm_mday); print(" ");
    print(timeinfo.tm_hour); print(":");
    print(timeinfo.tm_min); print(":");
    print(timeinfo.tm_sec); println("");
    //println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  }
  return okTime;
}


int nNet=0;
bool updateNTP() {
  bool bRet=false;
  int retryW=10;
  print("Connecting to ");
  print(ssid[nNet]);
  WiFi.begin(ssid[nNet].c_str(), pass[nNet].c_str());
  while(!bRet && retryW--) {
    print(".");
    delay(1000);
    blink();
    if(WiFi.status()==WL_CONNECTED) {
      println("");
      println(WiFi.SSID());
      print("Connected to WiFi network with IP Address: ");
      IPAddress ip=WiFi.localIP();
      println(ip.toString());
      int retryN=5;
      while(!bRet && retryN--) {
        bRet=updateNtpTime();
        if(!bRet)
          delay(1000);
        blink();
      }
      WiFi.disconnect();
    }
    if(!bRet) {
      nNet=(nNet+1)%MAX_NETWORKS;
    }
    else {
      while(WiFi.status()!=WL_DISCONNECTED) {
        delay(100);
        print(":");
        blink();
      }
    }
  }
  stopBlink();
  return bRet;
}
