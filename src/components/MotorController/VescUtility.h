#ifndef VescUtility_h
#define VescUtility_h

#include <Arduino.h> // for Serial.
#include "MotorController.h"
#include "VescParams.h"
#include "datatypes.h"



// TODO:: Implement these functions as memebr function on the MotorController.h class
// Now there is a limitation as the bldc_interface is expecting simple function to be
// passed as a parameter. Passing a class function memebr is not permitted.

void sendUartData(unsigned char *data, unsigned int len)
{
  // Writing data to serial.
  Serial.write(data, len);
}


void printValues(mc_values *val)
{
  VescParams::Instance()->setMotorValues(val);
}

void updateMotorController(MotorController* motorController)
{
  motorController->update();
}

#endif
