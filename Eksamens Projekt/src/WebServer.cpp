/*
 * With inspiration from:
 *
 * https://shawnhymel.com/1882/how-to-create-a-web-server-with-websockets-using-an-esp32-in-arduino/
 *
 * Author: J. Sanggaard
 * Date: 10. september 2020
 *
 */
#include <WebServer.h>

/***********************************************************
 * Functions
 */
callbackChange changeCallback;
callbackUpdate updateCallback;

/* comment out for router connection */
// #define SOFT_AP

const int32_t ws_port = 1337;
const int32_t led_pin = 17;

const char *cmd_forward = "forward";
const char *cmd_rotate = "rotate";
const char *cmd_toggle_location = "toggle_location";
const char *cmd_locationx = "locationx";
const char *cmd_locationy = "locationy";

// Globals
WebSocketsServer WebSocket = WebSocketsServer(ws_port);
TaskHandle_t WebSocketTaskHandle;
TaskHandle_t SyncTaskHandle;
// H_Bridge HBridge;

char MsgBuf[32];

void web_socket_send(const char *buffer, uint8_t client_num, bool broadcast)
{
  if (broadcast)
  {
    // log_d("Broadcasting: %s", buffer);
    WebSocket.broadcastTXT(buffer, strlen(buffer)); // all clients
  }
  else
  {
    log_d("Sending to [%u]: %s", client_num, buffer);
    WebSocket.sendTXT(client_num, buffer); // only one client
  }
}

void handle_forward(char *command, uint8_t client_num)
{
  char *value = strstr(command, ":");

  if (value == NULL || *value != ':')
  {
    log_e("[%u]: Bad command %s", client_num, command);
    return;
  }
  errno = 0;
  char *e;
  double result = strtol(value + 1, &e, 10);
  if (*e == '\0' && 0 == errno) // no error
  {
    changeCallback(&result, 'y');
  }
  else
  {
    log_e("[%u]: illegal forward state received: %s", client_num, value + 1);
  }
}

void handle_locationx(char *command, uint8_t client_num)
{
  char *value = strstr(command, ":");

  if (value == NULL || *value != ':')
  {
    log_e("[%u]: Bad command %s", client_num, command);
    return;
  }
  errno = 0;
  char *e;
  double result = strtol(value + 1, &e, 10);
  if (*e == '\0' && 0 == errno) // no error
  {
    changeCallback(&result, 'a');
  }
  else
  {
    log_e("[%u]: illegal location x state received: %s", client_num, value + 1);
  }
}

void handle_locationy(char *command, uint8_t client_num)
{
  char *value = strstr(command, ":");

  if (value == NULL || *value != ':')
  {
    log_e("[%u]: Bad command %s", client_num, command);
    return;
  }
  errno = 0;
  char *e;
  double result = strtol(value + 1, &e, 10);
  if (*e == '\0' && 0 == errno) // no error
  {
    changeCallback(&result, 'b');
  }
  else
  {
    log_e("[%u]: illegal location y state received: %s", client_num, value + 1);
  }
}

void handle_rotate(char *command, uint8_t client_num)
{
  char *value = strstr(command, ":");

  if (value == NULL || *value != ':')
  {
    log_e("[%u]: Bad command %s", client_num, command);
    return;
  }
  errno = 0;
  char *e;
  double result = strtol(value + 1, &e, 10);
  if (*e == '\0' && 0 == errno) // no error
  {
    // update rotate
    changeCallback(&result, 'x');
  }
  else
  {
    log_e("[%u]: illegal rotate state received: %s", client_num, value + 1);
  }
}

void handle_command(uint8_t client_num, uint8_t *payload, size_t length)
{
  char *command = (char *)payload;

  log_d("[%u] Received text: %s", client_num, command);

  if (strncmp(command, cmd_forward, strlen(cmd_forward)) == 0)
  {
    handle_forward(command, client_num);
  }
  else if (strncmp(command, cmd_rotate, strlen(cmd_rotate)) == 0)
  {
    handle_rotate(command, client_num);
  }
  else
  {
    log_e("[%u] Message not recognized", client_num);
  }

  WebSocket.connectedClients();
}

void handle_toggle_location(uint8_t client_num, uint8_t *payload, size_t length)
{
  char *command = (char *)payload;

  log_d("[%u] Received text: %s", client_num, command);

  if (strncmp(command, cmd_forward, strlen(cmd_toggle_location)) == 0)
  {
    // toggle between listening to location input to move input.
    char *value = strstr(command, ":");

    if (value == NULL || *value != ':')
    {
      log_e("[%u]: Bad command %s", client_num, command);
      return;
    }
    errno = 0;
    char *e;
    double result = strtol(value + 1, &e, 10);
    if (*e == '\0' && 0 == errno) // no error
    {
      changeCallback(&result, 'l');
    }
    else
    {
      log_e("[%u]: illegal location toggle state received: %s", client_num, value + 1);
    }
  }
  else
  {
    log_e("[%u] Message not recognized", client_num);
  }

  WebSocket.connectedClients();
}

void handle_location(uint8_t client_num, uint8_t *payload, size_t length)
{
  char *command = (char *)payload;

  log_d("[%u] Received text: %s", client_num, command);

  if (strncmp(command, cmd_forward, strlen(cmd_locationx)) == 0)
  {
    // Set wanted x location
    handle_locationx(command, client_num);
  }
  else if (strncmp(command, cmd_forward, strlen(cmd_locationy)) == 0)
  {
    // Set wanted y location
    handle_locationy(command, client_num);
  }
  else
  {
    log_e("[%u] Message not recognized", client_num);
  }

  WebSocket.connectedClients();
}

void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t *payload,
                      size_t length)
{
  // Figure out the type of WebSocket event
  switch (type)
  {

  // Client has disconnected
  case WStype_DISCONNECTED:
    log_i("[%u] Disconnected!", client_num);
    changeCallback(0, 'x');
    changeCallback(0, 'y');
    break;
  // New client has connected
  case WStype_CONNECTED:
  {
    IPAddress ip = WebSocket.remoteIP(client_num);
    log_i("[%u] Connection from ", client_num);
    log_i("IP: %s", ip.toString().c_str());
    break;
  }
  // Handle text messages from client
  case WStype_TEXT:
    handle_command(client_num, payload, length);
    break;

  // For everything else: do nothing
  case WStype_BIN:
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
  default:
    break;
  }
}

void setup_network(const char *ssid, const char *password)
{
#ifdef SOFT_AP
  // Start access point
  WiFi.softAP(ssid, password, wifi_channel); // (alle grupper skal bruge en unik kanal)
  // Print our IP address

  log_i("AP running");
  log_i("My IP address: ");
  log_i("IP: %s", WiFi.softAPIP().toString().c_str());

#else
  // connect to local network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    log_i("Establishing connection to WiFi..");
  }
  log_i("Connected to network");
  log_i("IP: %s", WiFi.localIP().toString().c_str());
#endif

  WebSocket.begin();
  WebSocket.onEvent(onWebSocketEvent);
}

void webSocketLoop(void *arg)
{
  TickType_t xTimeIncrement = configTICK_RATE_HZ / 10;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (true)
  {
    WebSocket.loop();
    vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
  }
}

void syncTask(void *arg)
{
  log_i("Loading");

  TickType_t xTimeIncrement = 1000;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (true)
  {
    double x = updateCallback('f');
    double y = updateCallback('r');
    double a = updateCallback('a');
    double lx = updateCallback('x');
    double ly = updateCallback('y');
    double lt = updateCallback('l');

    // Sync data in websocket
    sprintf(MsgBuf, "%s:%f", "xpos", x);
    web_socket_send(MsgBuf, 1, true);

    sprintf(MsgBuf, "%s:%f", "ypos", y);
    web_socket_send(MsgBuf, 1, true);

    sprintf(MsgBuf, "%s:%f", "angle", a);
    web_socket_send(MsgBuf, 1, true);

    // Location
    sprintf(MsgBuf, "%s:%f", "targetx", lx);
    web_socket_send(MsgBuf, 1, true);

    sprintf(MsgBuf, "%s:%f", "targety", ly);
    web_socket_send(MsgBuf, 1, true);

    // Location toggle
    sprintf(MsgBuf, "%s:%i", "locationtoggle", (int)lt);
    web_socket_send(MsgBuf, 1, true);

    vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
  }
}

void setup_tasks()
{
  log_i("starting websocket task");
  xTaskCreatePinnedToCore(
      webSocketLoop,
      "WebSocket_loop",
      10000, /* Stack size in words */
      NULL,  /* Task input parameter */
      25,    /* Priority of the task from 0 to 25, higher number = higher priority */
      &WebSocketTaskHandle,
      0); /* Core where the task should run */

  log_i("starting updatesync task");
  xTaskCreatePinnedToCore(
      syncTask,
      "UpdateSync_loop",
      10000, /* Stack size in words */
      NULL,  /* Task input parameter */
      1,     /* Priority of the task from 0 to 25, higher number = higher priority */
      &SyncTaskHandle,
      1); /* Core where the task should run */
}

void init_web(const char* SSID, const char* password, callbackChange onChange, callbackUpdate onUpdate)
{
  log_i("loading");

  //*LedState = &ledState;
  changeCallback = onChange;
  updateCallback = onUpdate;

  // Init LED and turn off
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  setup_network(SSID, password);
  setup_tasks();
}