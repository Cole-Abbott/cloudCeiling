// SECTION: INCLUDES

#include "neopixel.h"

// END SECTION: INCLUDES

// SECTION: GLOBALS

Adafruit_NeoPixel ceil_pixels(CEIL_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel desk_pixels(DESK_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

volatile enum neopixel_mode ceil_mode = OFF;
volatile enum neopixel_mode desk_mode = OFF;
volatile int brightness = BRIGHTNESS;
volatile uint32_t color = ceil_pixels.Color(255, 0, 0);

// END SECTION: GLOBALS

// SECTION: FUNCTION DEFINITIONS
void update_neopixel_task(void *pvParameters);

/**
    @brief   Sets up the neopixel and starts a freeRTOS task to handle the server loops
    @todo    should the taks be pinned to a core?
*/
void setup_neopixel()
{

    pinMode(LED_BUILTIN, OUTPUT);

    ceil_pixels.begin();
    ceil_pixels.setBrightness(BRIGHTNESS);
    ceil_pixels.show(); // Initialize all pixels to off

    desk_pixels.begin();
    desk_pixels.setBrightness(BRIGHTNESS);
    desk_pixels.show(); // Initialize all pixels to off

    // Start the neopixel task
    xTaskCreatePinnedToCore(
        update_neopixel_task,         // Task function
        "update_neopixel_task",       // Name of the task
        8192,                         // Stack size in words
        NULL,                         // Task input parameter
        2,                            // Priority of the task
        NULL,                         // Task handle
        CONFIG_ARDUINO_RUNNING_CORE); // Core where the task should run

    Serial.println("Neopixel setup complete");
}

/**
    @brief   Updates the neopixels with a new color based on the angle
    @todo    make this a freeRTOS task
*/
void update_neopixel_rainbow(Adafruit_NeoPixel &pixels)
{
    pixels.setBrightness(brightness);
    static int angle = 0;
    angle = (angle + 256) % 65536;
    for (int i = 0; i < pixels.numPixels(); i++)
    {
        u_int16_t pixelHue = angle + (i * 65536L / pixels.numPixels());
        pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }

    pixels.show();
}

void update_neopixel_solid(Adafruit_NeoPixel &pixels)
{
    pixels.setBrightness(brightness);
    pixels.fill(color, 0, pixels.numPixels());
    pixels.show();
}

void update_neopixel_off(Adafruit_NeoPixel &pixels)
{
    pixels.clear();
    pixels.show();
}



/**
    @brief   Task to handle neopixel updates
    @param   pvParameters FreeRTOS task parameters
*/
void update_neopixel_task(void *pvParameters)
{
    while (1)
    {
        switch (ceil_mode)
        {
        case RAINBOW:
            update_neopixel_rainbow(ceil_pixels);
            break;
        case SOLID:
            update_neopixel_solid(ceil_pixels);
            break;
        case OFF:
            update_neopixel_off(ceil_pixels);
            // do nothing, but must delay to allow other tasks to run
            break;
        }

        switch (desk_mode)
        {
        case RAINBOW:
            update_neopixel_rainbow(desk_pixels);
            break;
        case SOLID:
            update_neopixel_solid(desk_pixels);
            break;
        case OFF:
            update_neopixel_off(desk_pixels);
            break;
        }


        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

/**
    @brief   Sets the neopixel mode
    @param   new_mode The new mode to set
*/
void set_neopixel_mode(neopixel_mode new_mode, neopixel_strip strip)
{
    if (strip == CEIL)
    {
        ceil_mode = new_mode;
    }
    else if (strip == DESK)
    {
        desk_mode = new_mode;
    }
}


/**
    @brief   Gets the neopixel mode
*/
neopixel_mode get_neopixel_mode(neopixel_strip strip)
{
    if (strip == CEIL)
    {
        return ceil_mode;
    }
    else if (strip == DESK)
    {
        return desk_mode;
    }
}


/**
    @brief   Sets the neopixel brightness
    @param   new_brightness The new brightness to set
*/
void set_neopixel_brightness(int new_brightness)
{
    brightness = new_brightness;
}

/**
    @brief  Sets the neopixel color
    @param  r Red value
    @param  g Green value
    @param  b Blue value

 */
void set_neopixel_color(int r, int g, int b)
{
    color = ceil_pixels.gamma32(ceil_pixels.Color(r, g, b));
}

// END SECTION: FUNCTION DEFINITIONS