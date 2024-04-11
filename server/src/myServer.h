#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESPmDNS.h>
#include "filesystem.h"
#include "myNeopixels.h"

#define SSID "Device-Northwestern"
// #define PASSWORD "PASSWORD"

// Create AsyncWebServer object on port 80
extern WebServer server;
extern WebSocketsServer ws;


void start_web_services();