#include <global.h>
#include <WiFi.h>
//#include <WiFiClientSecure.h>
//#include <FS.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
//#include <hbridge.h>


typedef void (*callbackChange)(double *paramValue, char subtype);
typedef double (*callbackUpdate)(char subtype);

void init_web(callbackChange onChange, callbackUpdate onUpdate);
double get_pos(char type);