#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

typedef void (*commandCallback)(char *command, uint8_t client_num);
struct registeredCommand {
    char *command;
    commandCallback callback;
};

class WebServer
{
    public:
    WebServer(int32_t httpPort = 80, int32_t wsPort = 1337);

    void init(char *ssid, char *password);

    void registerCommand(char *command, commandCallback Command);

    private:
    registeredCommand registeredCommands[10];

    char* ssid;
    char* password;

    int32_t httpPort;
    int32_t wsPort;

    AsyncWebServer Server;
    TaskHandle_t WebSocketTaskHandle;
    WebSocketsServer WebSocket;

    void syncTask(void *args);
};