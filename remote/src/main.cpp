#include <Arduino.h>
#include "myClient.h"
#include "remote.h"


void setup() {
  Serial.begin(115200);
  connect_websocket();
  init_remote();
  
}

void loop() {

  // do nothing as all the work is done in tasks
  delay(100);
  
}