#include "remote.h"
#include "myClient.h"

// Function Declarations
void init_remote();
void button_isr();
void button_task(void *pvParameters);

// Global Variables

// Task Handle
TaskHandle_t button_task_handle = NULL;

// Function Definitions

/**
 * @brief Initializes the remote with an interupt for a button press and freertos task to handle the button press
 *
 */
void init_remote()
{
    // Set up the button
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_isr, FALLING);

    // Start task to handle button press
    xTaskCreatePinnedToCore(
        button_task,                  // Function to implement the task
        "button_task",                // Name of the task
        8192,                         // Stack size in words
        NULL,                         // Task input parameter
        3,                            // Priority of the task
        &button_task_handle,          // Task handle.
        CONFIG_ARDUINO_RUNNING_CORE); // Core where the task should run
}

/**
 * @brief ISR for button press
 *
 */
void IRAM_ATTR button_isr()
{
    // Notify the button task
    xTaskNotifyFromISR(button_task_handle, CEILING_TOGGLE, eSetValueWithOverwrite, NULL);
}

/**
 * @brief Task to handle the toggle ceiling button press
 *
 */
void button_task(void *pvParameters)
{
    Serial.println("Button Task Started");

    for (;;)
    {
        // Wait for the button press notification
        uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        Serial.println("Button Pressed");

        switch (notification)
        {
        case CEILING_TOGGLE:
            // Toggle the ceiling light
            if (state["ceiling"]["mode"] == "solid")
            {
                state["ceiling"]["mode"] = "off";
            }
            else
            {
                state["ceiling"]["mode"] = "solid";
            }
            break;
        }

        send_state(); // Send the new state to the server
        delay(50); // Debounce

    }
}