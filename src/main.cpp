#include <Arduino.h>
#include "FMV.h"

FMV fmv;

void setup() {
  Serial.begin(115200);
  fmv.setup();;

}
void loop() {
  fmv.loop();
  yield();
}
