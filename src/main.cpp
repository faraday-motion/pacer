#include <Arduino.h>
#include "FMV.h"
#include "components/Console/Console.h"

FMV fmv;

void setup() {
  Serial.begin(115200);
  fmv.setup();;

}
void loop() {
  fmv.loop();
  yield();
}
