#include "myNeopixels.h"
#include "filesystem.h"
#include "myServer.h"

#define STATUS_PIN 13
#define STATUS_PIN_2 12

void setup()
{
  Serial.begin(115200);
  delay(1000); 

  // Initialize SPIFFS
  uint8_t spiffs_check = startSPIFFS();
  if (spiffs_check == 1)
  {
    Serial.println("SPIFFS ERROR!");
    return;
  }

  // initlalise a gpio pin for processing status
  pinMode(STATUS_PIN, OUTPUT);
  pinMode(STATUS_PIN_2, OUTPUT);

  setup_neopixel();
  start_web_services();

}

// do nothing as everything is handled in tasks
void loop()
{
  // Serial.println("loop");
  vTaskDelay(5 / portTICK_PERIOD_MS);
}
