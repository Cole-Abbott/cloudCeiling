// SECTION: INCLUDES

#include "neopixel.h"

// END SECTION: INCLUDES



// SECTION: NEOPIXEL CLASS DEFINITION

// Constructor
Neopixel::Neopixel(int pin, int num_pixels)
{
    pixels = new Adafruit_NeoPixel(num_pixels, pin, NEO_GRB + NEO_KHZ800);
    pixels->begin();
    mode = OFF;
    brightness = DEFAULT_BRIGHTNESS;
    color = pixels->Color(255, 0, 0);
    pixels->show(); // Initialize all pixels to off
}

// Default constructor
Neopixel::Neopixel()
{
}

void Neopixel::set_mode(neopixel_mode new_mode)
{
    mode = new_mode;
}

void Neopixel::set_brightness(int new_brightness)
{
    brightness = new_brightness;
}

void Neopixel::set_color(int r, int g, int b)
{
    color = pixels->Color(r, g, b);
}

// Update the neopixels based on the mode
void Neopixel::update() {
    pixels->setBrightness(brightness);
    switch (mode)
    {
    case RAINBOW:
        update_rainbow();
        break;
    case SOLID:
        update_solid();
        break;
    case FLASH:
        update_flash();
        break;
    case OFF:
        update_off();
        break;
    default:
        break;
    }
    pixels->show();
}

// Update the neopixels with a new color based on the angle
void Neopixel::update_rainbow()
{
    static int angle = 0;
    angle = (angle + 256) % 65536;
    for (int i = 0; i < pixels->numPixels(); i++)
    {
        pixels->setPixelColor(i, pixels->gamma32(pixels->ColorHSV(angle + (i * 65536 / pixels->numPixels()))));
    }
}

// Update the neopixels with a solid color
void Neopixel::update_solid()
{
    pixels->fill(color, 0, pixels->numPixels());
}

// Update the neopixels with a flashing color
void Neopixel::update_flash()
{
   //implement this later
}

// Turn off the neopixels
void Neopixel::update_off()
{
    pixels->clear();
}



// END SECTION: NEOPIXEL CLASS DEFINITION