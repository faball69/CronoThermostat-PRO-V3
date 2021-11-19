/*
 * web service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

WebServer server(80);

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void _handleRoot(const char *message) {
  char temp[5000];
  memset(temp, 0, sizeof(temp));
  snprintf(temp, 5000,
    "<html><body>\
    <script>if(typeof window.history.pushState == 'function') {window.history.pushState({}, \"Hide\", \"/\");}</script>\
    <h1>Plant%d</h1>\
    <strong>Temperature: </strong> %.1f <strong> Time: </strong> %.02d:%.02d <br>\
    <strong>Heater: </strong> %s <strong>Force: </strong> %s <br><br>\
    <table style=\"border-collapse: collapse; width: 100%%;\" border=\"1\">\
    <tbody>\
    <tr>\
    <td style=\"width: 12.5%%;\">\
    <form action=\"/get\">\
      offset[deg]:<input min=\"-7.0\" max=\"7.0\" name=\"PF1\" step=\"0.1\" type=\"number\" value=\"%.1f\" /><br><br>\
      tempLow[deg]:<input min=\"15.0\" max=\"20.0\" name=\"PF2\" step=\"0.1\" type=\"number\" value=\"%.1f\" /><br><br>\
      TempHigh[deg]:<input min=\"15.0\" max=\"23.0\" name=\"PF3\" step=\"0.1\" type=\"number\" value=\"%.1f\" /><br><br>\
      Hysteresis[deg]:<input min=\"0.0\" max=\"0.5\" name=\"PF4\" step=\"0.1\" type=\"number\" value=\"%.1f\" /><br><br>\
      timeStart[hh:mm]:<input type=\"time\" name=\"TS\" value=\"%.02d:%.02d\" /><br><br>\
      timeEnd[hh:mm]:<input type=\"time\" name=\"TE\" value=\"%.02d:%.02d\" /><br><br>\
      nCh:<input min=\"-1\" max=\"1\" type=\"number\" name=\"CH\" value=\"%d\" /> -1=APmode 0=Heater1 1=Heater2<br><br><br>\
    <input type=submit value=Save>Parameters</form><br>\
    </td>\
    <td style=\"width: 12.5%%;\">\
    <form action=\"/force\"><input type=submit value=Force>heater ON for 30min from now!</form><br><br><br>\
    <form action=\"/reset\"><input type=submit value=Reset>ESP32 Board to restart! Pay Attention!</form><br><br><br>\
    <form action=\"/plant\">now Plant is %s <input type=submit value=Turn%s></form>\
    </td>\
    </tr>\
    </tbody>\
    </table>\
    <br><strong> %s </strong><br>\
    <br><strong> debugMsg: </strong> %s \
    </body></html>\n", plant.nCh+1, ambT, timeNow.tm_hour, timeNow.tm_min, (state?"ON":"OFF"), (force?"ON":"OFF"),
                       plant.tOffset, plant.tLow, plant.tHigh, plant.tHyst, plant.minIni/60, plant.minIni%60, plant.minFin/60, plant.minFin%60, plant.nCh,
                       (plant.bEnable?"ON":"OFF"), (plant.bEnable?"OFF":"ON"), message, debugMsg.c_str());
  server.send(200, "text/html", temp);
}

void handleRoot() {
  _handleRoot("");
}

void handleReset() {
  _handleRoot("ESP32 resetted! Wait a little bit....!");
  reset=true;
}

void handleForce() {
  _handleRoot("Force heater ON for 30min!");
  if(plant.bEnable)
    force=true;
}

void handlePlant() {
  plant.bEnable=!plant.bEnable;
  if(plant.bEnable)
    _handleRoot("Now plant is enabled!");
  else {
    _handleRoot("Now plant is disabled!");
    force=false;
  }
  saveData();
}

void handleSetPar() {
  // for all pars
  // http://192.168.2.103/?PP1=10&PF1=28.0&TS1=14:00&TS2=14:00&PP2=5&PF2=0.5&TE1=20:00&TE2=20:00&E1=on&E2=on&E3=on&E4=on
  for(int i=0;i<server.args();i++) {
    String txtPar;
    txtPar=server.arg("PF1");
    if(txtPar!="")
      plant.tOffset=txtPar.toFloat();
    txtPar=server.arg("PF2");
    if(txtPar!="")
      plant.tLow=txtPar.toFloat();
    txtPar=server.arg("PF3");
    if(txtPar!="")
      plant.tHigh=txtPar.toFloat();
    txtPar=server.arg("PF4");
    if(txtPar!="")
      plant.tHyst=txtPar.toFloat();
    txtPar=server.arg("TS");
    if(txtPar!="")
      plant.minIni=txtPar.substring(0,2).toInt()*60+txtPar.substring(3).toInt();
    txtPar=server.arg("TE");
    if(txtPar!="")
      plant.minFin=txtPar.substring(0,2).toInt()*60+txtPar.substring(3).toInt();
    txtPar=server.arg("CH");
    if(txtPar!="") {
      int newInt=txtPar.toInt();
      if(newInt!=plant.nCh) {
        plant.nCh=newInt;
        firstLoop=true;
      }
    }
  }
  saveData();
  _handleRoot("Parameters saved!");
}

void initWeb() {
  println("");
  if (MDNS.begin("esp32")) {
    println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/get", handleSetPar);
  server.on("/force", handleForce);
  server.on("/reset", handleReset);
  server.on("/plant", handlePlant);
  server.onNotFound(handleNotFound);
  server.begin();
  println("HTTP server started");
}

 void handleWeb() {
   server.handleClient();
 }
