#pragma once

// SECTION: INCLUDES
#include "neopixel.h"
#include <map>
#include <Arduino.h>
#include <ArduinoJson.h>
// END SECTION: INCLUDES

// SECTION: NEOPIXELS CLASS DEFINITION

class Neopixels
{
private:
    std::map<String, Neopixel *> strips;
    JsonDocument state;

public:
    Neopixels();
    void add_strip(String name, int pin, int num_pixels);
    void set_mode(String name, String new_mode);
    void set_brightness(String name, int new_brightness);
    void set_color(String name, int r, int g, int b);
    void update();
    void parse_command(char *payload);
    void print_state();
    void send_state();
};