#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESPmDNS.h>
#include "filesystem.h"
#include "myNeopixels.h"

#define SSID "Device-Northwestern"
// #define SSID "Cole's iPhone"
// #define PASSWORD "Keiko1818"

// Create AsyncWebServer object on port 80
extern WebServer server;
extern WebSocketsServer ws;


void start_web_services();