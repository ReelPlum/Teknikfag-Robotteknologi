#include <global.h>
#include <WiFi.h>
//#include <WiFiClientSecure.h>
//#include <FS.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
//#include <hbridge.h>

/* comment out for router connection */
#define SOFT_AP

/* Constants */
#ifdef SOFT_AP
const char *ssid = "Robot";
const char *password = "esp32esp32";
#else
const char *ssid = "MakitaNG";
const char *password = "...";
#endif

const char *cmd_toggle = "toggle";
const char *cmd_led_state = "led_state";
const char *cmd_sli = "sli";
const char *cmd_pid = "pid_";

const int32_t wifi_channel = 5; // alle grupper skal have hver sin kanal
const int32_t dns_port = 53;
const int32_t http_port = 80;
const int32_t ws_port = 1337;
const int32_t led_pin = 17;

// Globals
AsyncWebServer Server(http_port);
WebSocketsServer WebSocket = WebSocketsServer(ws_port);
TaskHandle_t WebSocketTaskHandle;
//H_Bridge HBridge;

char MsgBuf[32];

typedef void (*callbackChange)(double *paramValue, char subtype);
typedef void (*callbackUpdate)(double *value, char subtype);

void init_web(callbackChange onChange);