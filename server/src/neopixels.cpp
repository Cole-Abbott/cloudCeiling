// SECTION: INCLUDES
#include "neopixels.h"
#include <ArduinoJson.h>
#include "myServer.h"

// END SECTION: INCLUDES

// SECTION: NEOPIXELS CLASS DEFINITION

Neopixels::Neopixels()
{
}

void Neopixels::add_strip(String name, int pin, int num_pixels)
{
    // strips[name] = Neopixel(pin, num_pixels);
    strips[name] = new Neopixel(pin, num_pixels);

    if (strips[name] == NULL)
    {
        Serial.println("Failed to allocate memory for neopixel strip");
    }

    // add the strip to the state
    state[name]["mode"] = "off";
    state[name]["brightness"] = 100;
    state[name]["color"]["r"] = 0;
    state[name]["color"]["g"] = 0;
    state[name]["color"]["b"] = 0;
}

void Neopixels::set_mode(String name, String new_mode)
{
    // check if the strip exists
    if (strips.find(name) == strips.end())
    {
        Serial.printf("Neopixel strip %s not found\n", name.c_str());
        return;
    }
    if (new_mode == "rainbow")
    {
        strips[name]->set_mode(RAINBOW);
    }
    else if (new_mode == "off")
    {
        strips[name]->set_mode(OFF);
    }
    else if (new_mode == "solid")
    {
        strips[name]->set_mode(SOLID);
    }
    else if (new_mode == "flash")
    {
        strips[name]->set_mode(FLASH);
    }
    else if (new_mode == "twinkle")
    {
        strips[name]->set_mode(TWINKLE);
    }

    // update the state
    state[name]["mode"] = new_mode;
}

void Neopixels::set_brightness(String name, int new_brightness)
{
    strips[name]->set_brightness(new_brightness);
    state[name]["brightness"] = new_brightness;
}

void Neopixels::set_color(String name, int r, int g, int b)
{
    strips[name]->set_color(r, g, b);
    state[name]["color"]["r"] = r;
    state[name]["color"]["g"] = g;
    state[name]["color"]["b"] = b;
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

// parse the command from the websocket
void Neopixels::parse_command(char *payload)
{
    // //String strip;
    // char strip[20];

    // //parse JSON
    JsonDocument doc;
    deserializeJson(doc, payload);


    // loop through the strips
    for (auto &S : strips)
    {
        // check if the strip exists
        if (doc[S.first] == nullptr)
        {
            Serial.printf("Neopixel strip %s not found\n", S.first.c_str());
            continue;
        }

        // set the mode
        set_mode(S.first, doc[S.first]["mode"]);

        // set the brightness
        set_brightness(S.first, doc[S.first]["brightness"].as<int>());

        // set the color
        set_color(S.first, doc[S.first]["color"]["r"].as<int>(), doc[S.first]["color"]["g"].as<int>(), doc[S.first]["color"]["b"].as<int>());
    }

    // send the new state to the websocket
    send_state();

}

// print the state of the neopixels to the serial monitor
void Neopixels::print_state()
{
    // serialize the state
    String output;
    serializeJsonPretty(state, output);
    Serial.println(output);
}

// send the state of the neopixels to the websocket
void Neopixels::send_state()
{
    // serialize the state
    String output;
    serializeJsonPretty(state, output);
    ws.broadcastTXT(output);
}
