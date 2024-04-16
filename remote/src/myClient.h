#include <Arduino.h> // Arduino Core for ESP32
#include <WiFi.h> // WiFi Library for ESP32
#include <WebServer.h> // WebServer Library for ESP32
#include <WebSocketsClient.h> // WebSocket Client Library for WebSocket
#include <ArduinoJson.h> // Arduino JSON Library


#define SSID "Device-Northwestern" // WiFi SSID
#define SERVER_IP "10.106.6.244" // Server IP Address

// Create a WebSocket object
extern WebSocketsClient webSocket;
extern JsonDocument state;

//function prototypes
void connect_websocket();
void send_state();