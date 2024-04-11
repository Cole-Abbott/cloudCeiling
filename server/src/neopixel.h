#pragma once

// SECTION: INCLUDES

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

// END SECTION: INCLUDES


// SECTION: DEFINES

#define DEFAULT_BRIGHTNESS 100

// END SECTION: DEFINES


// SECTION: ENUMS
enum neopixel_mode
{
    RAINBOW,
    SOLID,
    FLASH,
    TWINKLE,
    OFF
};
// END SECTION: ENUMS


// SECTION: NEOPIXEL CLASS DECLARATION

class Neopixel
{
    private:
        Adafruit_NeoPixel* pixels;
        volatile enum neopixel_mode mode;
        volatile int brightness;
        volatile uint32_t color;

    public:
        Neopixel(int pin, int num_pixels);
        Neopixel();
        void set_mode(neopixel_mode new_mode);
        void set_brightness(int new_brightness);
        void set_color(int r, int g, int b);

        void update_rainbow();
        void update_solid();
        void update_flash();
        void update_twinkle();
        void update_off();

        void update();


};