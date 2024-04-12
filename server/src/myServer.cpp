#include "myServer.h"


// Create AsyncWebServer object on port 80
WebServer server(80);
WebSocketsServer ws(81);


const char* mdns_name = "smarthome";

void server_loop_task(void *pvParameters);
void connect_wifi();
void startMDNS();
void start_server();
void startWebSocket();

/**
 * @brief Connects to wifi and starts the server
 *
 */
void start_web_services()
{
    connect_wifi();
    startMDNS();
    start_server();
    startWebSocket();

    // Start task to handle server loops
    xTaskCreatePinnedToCore(
        server_loop_task,             // Function to implement the task
        "server_loops",               // Name of the task
        8192,                         // Stack size in words
        NULL,                         // Task input parameter
        3,                            // Priority of the task
        NULL,                         // Task handle.
        CONFIG_ARDUINO_RUNNING_CORE); // Core where the task should run

    Serial.println("Web services started.");
} // end start_web_services


/**
 * @brief starts the mDNS responder
 * 
 */
void startMDNS() { // Start the mDNS responder
  MDNS.begin(mdns_name);                        // start the multicast domain name server
  Serial.print("mDNS responder started: http://");
  Serial.print(mdns_name);
  Serial.println(".local");
} // end startMDNS

/**
 * @brief Connects to wifi
 *
 */
void connect_wifi()
{
    //print mac address
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
 * @brief Starts the server and the server loop task
 *
 */
void start_server()
{
    // list directory
    server.on("/list", HTTP_GET, handleFileList);
    // load editor
    server.on("/edit", HTTP_GET, []()
              {
    if (!handleFileRead("/edit.html")) {
      server.send(404, "text/plain", "FileNotFound");
    } });
    // create file
    server.on("/edit", HTTP_PUT, handleFileCreate);
    // delete file
    server.on("/edit", HTTP_DELETE, handleFileDelete);
    // first callback is called after the request has ended with all parsed arguments
    // second callback handles file uploads at that location
    server.on(
        "/edit", HTTP_POST, []()
        { server.send(200, "text/plain", ""); },
        handleFileUpload);

    // add handler for root file
    server.on("/", HTTP_GET, []()
              { handleFileRead("/"); });

    // add handler for each file
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    { // List the file system contents
        String fileName = file.name();
        server.on("/" + fileName, HTTP_GET, []()
                  { handleFileRead(server.uri()); });
        file = root.openNextFile();
    }

    server.onNotFound(handleNotFound); // if someone requests any other file or page, go to function 'handleNotFound'
                                       // and check if the file exists

    server.begin(); // start the HTTP server
    Serial.println("HTTP server started.");
} // end start_server

/**
 * @brief Task to handle server loops
 *
 * @param pvParameters FreeRTOS task parameters
 */
void server_loop_task(void *pvParameters)
{
    Serial.println("Server loop task started.");
    while (1)
    {
        #define STATUS_PIN 13
        // turn on status pin
        digitalWrite(STATUS_PIN, HIGH);
        ws.loop();
        server.handleClient();
        // turn off status pin
        digitalWrite(STATUS_PIN, LOW);
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
} // end server_loop_task

/**
 * @brief Handles websocket messages
 *
 * @param num Websocket client number
 * @param type Websocket message type
 * @param payload Websocket message payload
 * @param len Websocket message length
 */
void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t len)
{
    // Serial.printf("WebSocket client #%u message\r\n", num);
    // Serial.printf("Type: %u\r\n", type);
    // Serial.printf("Payload: %s\r\n", payload);
    // Serial.printf("Length: %u\r\n", len);

    //handle text message by changing neopixel mode
    if (type == WStype_TEXT)
    {
        neopixels.parse_command((char *)payload);
        Serial.printf("Received command: %s\r\n", payload);
    }
} // end handleWebSocketMessage

/**
 * @brief Handles websocket events
 *
 * @param num Websocket client number
 * @param type Websocket event type
 * @param payload Websocket event payload
 * @param len Websocket event length
 */
void onEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t len)
{
    static uint32_t websocket_conn_count = 0;

    IPAddress ip = ws.remoteIP(num);

    switch (type)
    {
    case WStype_CONNECTED:
        Serial.printf("WebSocket client #%u connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        websocket_conn_count = ws.connectedClients();
        Serial.printf("Total connected clients: %u\r\n", websocket_conn_count);
        break;
    case WStype_DISCONNECTED:
        Serial.printf("WebSocket client #%u disconnected\r\n", num);
        websocket_conn_count = ws.connectedClients();
        Serial.printf("Total connected clients: %u\r\n", websocket_conn_count);
        if (websocket_conn_count == 0)
        {
            Serial.println("No connected clients");
        }
        break;
    case WStype_TEXT:
        handleWebSocketMessage(num, type, payload, len);
        break;
    case WStype_PONG:
    case WStype_ERROR:
        break;
    }
} // end onEvent

void startWebSocket()
{                        // Start a WebSocket server
    ws.begin();          // start the websocket server
    ws.onEvent(onEvent); // if there's an incomming websocket message, go to function 'webSocketEvent'
    Serial.println("WebSocket server started.");
} // end startWebSocket