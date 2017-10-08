#include <Arduino.h>
#include "FMV.h"
#include "components/MotorController/MotorController.h"
#include "components/Console/Console.h"

#define DEAD_MAN_SWITCH 2

FMV fmv;
bool DEAD_MAN_SWITCH_ENABLED = true;

// Redundant Direct Connection to VESC
MotorController mc;

void handleDeadManSwitchInterrupt() {
  DEAD_MAN_SWITCH_ENABLED = digitalRead(DEAD_MAN_SWITCH);
}

void setup() {
  Serial.begin(115200);
  pinMode(DEAD_MAN_SWITCH, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEAD_MAN_SWITCH), handleDeadManSwitchInterrupt, CHANGE);
  mc.setup();
  fmv.setup();

}
void loop() {
  if (!DEAD_MAN_SWITCH_ENABLED) {
    fmv.loop();
  } else {
    // Setting neutral current.
    mc.set_current_brake(0);

    Serial.println("::::::::PANIC:::::::::");
    Serial.println("DEAD_MAN_SWITCH_ENABLED == TRUE");
  }
  yield();
}
