// SECTION: INCLUDES

#include "neopixel.h"
#include <map>

// END SECTION: INCLUDES

// SECTION: HELPER FUNCTIONS
uint32_t fadeColor(uint32_t color, int intensity);
// END SECTION: HELPER FUNCTIONS

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
void Neopixel::update()
{
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
    case TWINKLE:
        update_twinkle();
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
    // implement this later
}

// Update the neopixels with a twinkling effect
void Neopixel::update_twinkle()
{
    // this function is called every ~10ms, so we can use a static variable to keep track of the time
    static int count = 0;
    static std::map<int, std::pair<uint32_t, int>> on_pixels; // keep track of which pixels are on: <pixel, <color, intensity>>

    if (on_pixels.size() < 20 && count % 10 == 0) // add a new pixel every 100ms, up to 20 pixels
    {
        // pick a random pixel to turn on to a random color
        int pixel = random(0, pixels->numPixels());
        on_pixels[pixel].first = pixels->Color(random(0, 255), random(0, 255), random(0, 255));
        on_pixels[pixel].second = random(100, 255);
    }
    count++;

    pixels->clear();
    // update the pixels scaled by their intensithy
    for (auto &it : on_pixels)
    {
        if (it.second.second <= 0) // if the pixel is off, remove it from the map
        {
            on_pixels.erase(it.first);
        }
        else
        {
            it.second.first = fadeColor(it.second.first, it.second.second);    // fade the color
            pixels->setPixelColor(it.first, pixels->gamma32(it.second.first)); // set the pixel color
        }
        it.second.second -= 1; // decrease the intensity
    }
} // update_twinkle

// Turn off the neopixels
void Neopixel::update_off()
{
    pixels->clear();
}

// END SECTION: NEOPIXEL CLASS DEFINITION

// SECTION: HELPER FUNCTIONS

// Fade a color by a certain intensity (0-255)
uint32_t fadeColor(uint32_t color, int intensity)
{
    int r = (color >> 16) & 0xFF / 255 * intensity;
    int g = (color >> 8) & 0xFF / 255 * intensity;
    int b = color & 0xFF / 255 * intensity;
    return (r << 16) | (g << 8) | b;
} // fadeColor

// END SECTION: HELPER FUNCTIONS
