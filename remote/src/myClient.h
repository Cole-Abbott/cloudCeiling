#include <Arduino.h> // Arduino Core for ESP32
#include <WiFi.h> // WiFi Library for ESP32
#include <WebServer.h> // WebServer Library for ESP32
#include <WebSocketsClient.h> // WebSocket Client Library for WebSocket
#include <ArduinoJson.h> // Arduino JSON Library


#define SSID "Device-Northwestern" // WiFi SSID

// Create a WebSocket object
extern WebSocketsClient webSocket;


//function prototypes
void connect_websocket();