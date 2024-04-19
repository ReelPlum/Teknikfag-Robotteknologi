#include <global.h>
#include <WiFi.h>
//#include <WiFiClientSecure.h>
//#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>


typedef void (*callbackChange)(double *paramValue, char subtype);
typedef double (*callbackUpdate)(char subtype);

void init_web(char* SSID, char* password, callbackChange onChange, callbackUpdate onUpdate);