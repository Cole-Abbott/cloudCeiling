// SECTION: INCLUDES
#include "neopixel.h"
#include "neopixels.h"
// END SECTION: INCLUDES

// SECTION: DEFINES

// #define ESP32_C3


#define CEIL_PIN 32
#define CEIL_PIXELS 300

#define DESK_PIN 33
#define DESK_PIXELS 55

#define DEFAULT_BRIGHTNESS 100


//for esp32-c3 the LED_BUILTIN is 8 and led pin is 2
#ifdef ESP32_C3
    #undef CEIL_PIN
    #define CEIL_PIN 2
    #undef DESK_PIN
    #define DESK_PIN 3
#endif

// END SECTION: DEFINES

// SECTION: GLOBAL VARIABLES

extern Neopixels neopixels;

// END SECTION: GLOBAL VARIABLES

// SECTION: FUNCTION DECLARATIONS

void setup_neopixel();

// END SECTION: FUNCTION DECLARATIONS