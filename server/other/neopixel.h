// SECTION: INCLUDES

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

// END SECTION: INCLUDES

// SECTION: DEFINES

// #define ESP32_C3


#define LED_PIN 32
#define CEIL_PIXELS 300
#define DESK_PIXELS 55
#define BRIGHTNESS 100


//for esp32-c3 the LED_BUILTIN is 8 and led pin is 2
#ifdef ESP32_C3
    #undef LED_BUILTIN
    #define LED_BUILTIN 8
    #undef LED_PIN
    #define LED_PIN 2
#endif

// END SECTION: DEFINES

// SECTION: ENUMS
enum neopixel_mode
{
    RAINBOW,
    SOLID,
    FLASH,
    OFF
};

enum neopixel_strip
{
    CEIL,
    DESK
};



// SECTION: FUNCTION DECLARATIONS

void setup_neopixel();
neopixel_mode get_neopixel_mode();
void set_neopixel_mode(neopixel_mode new_mode);
void set_neopixel_brightness(int new_brightness);
void set_neopixel_color(int r, int g, int b);




// END SECTION: FUNCTION DECLARATIONS
