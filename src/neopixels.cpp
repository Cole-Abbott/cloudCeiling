// SECTION: INCLUDES
#include "neopixels.h"
// END SECTION: INCLUDES

// SECTION: NEOPIXELS CLASS DEFINITION

Neopixels::Neopixels()
{
}

void Neopixels::add_strip(String name, int pin, int num_pixels)
{
    //strips[name] = Neopixel(pin, num_pixels);
    strips[name] = new Neopixel(pin, num_pixels);

    if (strips[name] == NULL)
    {
        Serial.println("Failed to allocate memory for neopixel strip");
    }

}

void Neopixels::set_mode(String name, neopixel_mode new_mode)
{
    // check if the strip exists
    if (strips.find(name) == strips.end())
    {
        Serial.printf("Neopixel strip %s not found\n", name.c_str());
        return;
    }
    strips[name]->set_mode(new_mode);
}

void Neopixels::set_brightness(String name, int new_brightness)
{
    strips[name]->set_brightness(new_brightness);
}

void Neopixels::set_color(String name, int r, int g, int b)
{
    strips[name]->set_color(r, g, b);
}

// Update all the neopixel strips
void Neopixels::update()
{
    for (auto &S : strips)
    {
        S.second->update();
    }
    // strips["ceiling"]->update();
    // strips["desk"]->update();
}

//parse the command from the websocket
void Neopixels::parse_command(char *payload)
{
    //String strip;
    char strip[20];

    if (strstr((char *)payload, "rainbow"))
    {
        sscanf((char *)payload, "rainbow %s", &strip);
        set_mode(strip, RAINBOW);
    }
    else if (strstr((char *)payload, "off"))
    {
        sscanf((char *)payload, "off %s", &strip);
        set_mode(strip, OFF);
    }
    else if (strstr((char *)payload, "solid"))
    {
        sscanf((char *)payload, "solid %s", &strip);
        set_mode(strip, SOLID);
    }
    else if (strstr((char *)payload, "brightness"))
    {
        int brightness;
        sscanf((char *)payload, "brightness %s %d", &strip, &brightness);
        set_brightness(strip, brightness);
    }
    else if (strstr((char *)payload, "color"))
    {
        int r, g, b;
        sscanf((char *)payload, "color %s %d %d %d", &strip, &r, &g, &b);
        set_color(strip, r, g, b);
    }
    else
    {
        Serial.printf("Unknown command: %s\n", payload);
    }
}
