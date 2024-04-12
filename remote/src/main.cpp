#include <Arduino.h>
#include <myClient.h>


void setup() {
  Serial.begin(115200);
  connect_websocket();
  
}

void loop() {

  // do nothing as all the work is done in tasks
  delay(100);
  
}