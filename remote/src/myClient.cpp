#include <myClient.h>


//Function Declarations
void connect_wifi();
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
void client_loop_task(void *pvParameters);

WebSocketsClient webSocket;


/**
 * @brief Connects to the websocket
 *
 */
void connect_websocket()
{
    connect_wifi();

    // Create a WebSocket object
    // server address, port and URL
    webSocket.begin("10.106.6.244", 81, "/");

    // event handler
    webSocket.onEvent(webSocketEvent);

    Serial.println("WebSocket Client Connected");

    // start freertos task to handle websocket events
    // Start task to handle server loops
    xTaskCreatePinnedToCore(
        client_loop_task,             // Function to implement the task
        "client_loops",               // Name of the task
        8192,                         // Stack size in words
        NULL,                         // Task input parameter
        3,                            // Priority of the task
        NULL,                         // Task handle.
        CONFIG_ARDUINO_RUNNING_CORE); // Core where the task should run

}



/**
 * @brief Connects to wifi
 *
 */
void connect_wifi()
{
    // print mac address
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());

    // connect to Wi-Fi
#ifdef PASSWORD
    WiFi.begin(SSID, PASSWORD);
#endif
#ifndef PASSWORD
    WiFi.begin(SSID);
#endif

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to " + String(SSID));
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
} // end connect_wifi

/**
 * @brief Connects to the websocket
 *
 */
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected!\n");
        break;
    case WStype_CONNECTED:
        Serial.printf("[WSc] Connected to url: %s\n", payload);
        break;
    case WStype_TEXT:
        Serial.printf("[WSc] get text: %s\n", payload);
        break;
    case WStype_BIN:
        Serial.printf("[WSc] get binary length: %u\n", length);
        break;
    }
} // end webSocketEvent

/**
 * @brief Task to handle websocket events
 *
 * @param pvParameters
 */
void client_loop_task(void *pvParameters)
{
    while(1)
    {
        webSocket.loop();
        delay(50);
    }
} // end client_loop_task