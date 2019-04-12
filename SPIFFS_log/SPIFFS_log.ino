#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <WebServer.h>

#define maxFileSize 20*1024 //para evitar overflow de datos, dejo de espacio libre minimo el tamaño de un fichero entero

WebServer server(80);
int totalSize = 0;
bool doneMax = false;

void setup(){
    Serial.begin(115200);
    if(!SPIFFS.begin(true)){ //true si quieres formatear el sistema de archivos si falla o no existe
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    //SPIFFS.format(); //usar solo para borrar sistema entero
    
  WiFi.softAP("CanSat", "pakitoor"); //SSID y password
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("HotSpot IP: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/get", handleGetFile);
  server.onNotFound([]() {
    server.send(200, "text/plain", "Not found on server");
  });
  server.begin();
}

int test = 0;
int interrupt = 0;
int logNum = -1;


void loop(){
  yield();
  server.handleClient();
  if((millis()-interrupt)>1000){
    interrupt = millis();
    test++;
    String num;
    num+=test;
    num = num +" @527 135006 8 53,1255928 9,3524972 213,7 12,4 1,06 10,03 -86,83 5 37,71 975,72 372,84 -2,07 540,16 1,16 0,53 0,54 122 290,16\n";
    num = num + num;
    logData(num);
    }
}
