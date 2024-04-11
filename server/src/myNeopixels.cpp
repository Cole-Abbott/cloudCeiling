#include "myNeopixels.h"


// SECTION: GLOBAL VARIABLES
Neopixels neopixels;

// SECTION: FUNCTION DEFINITIONS
void update_neopixel_task(void *pvParameters);

void setup_neopixel()
{
    // Initialize the neopixel strips
    neopixels.add_strip("ceiling", CEIL_PIN, CEIL_PIXELS);
    neopixels.add_strip("desk", DESK_PIN, DESK_PIXELS);

    // Start the neopixel task
    xTaskCreatePinnedToCore(
        update_neopixel_task,         // Task function
        "update_neopixel_task",       // Name of the task
        8192,                         // Stack size in words
        NULL,                         // Task input parameter
        2,                            // Priority of the task
        NULL,                         // Task handle
        CONFIG_ARDUINO_RUNNING_CORE); // Core where the task should run
    Serial.println("Neopixels setup complete");
}


void update_neopixel_task(void *pvParameters)
{
    Serial.println("Neopixel loop task started");
    while(1)
    {
        #define STATUS_PIN 12
        digitalWrite(STATUS_PIN, HIGH);
        neopixels.update();
        digitalWrite(STATUS_PIN, LOW);
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}