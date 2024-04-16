#include <Arduino.h>
#include "myClient.h"


//DEFINES
#define BUTTON_PIN 3 // Button Pin


//ENUMS
enum Buttons {
    CEILING_TOGGLE,
    DESK_TOGGLE
};

//Function Declarations

void init_remote();