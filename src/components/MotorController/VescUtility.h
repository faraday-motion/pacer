#ifndef VescUtility_h
#define VescUtility_h

#include <Arduino.h> // for Serial.
#include "MotorController.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/datatypes.h"



// TODO:: Implement these functions as memebr function on the MotorController.h class
// Now there is a limitation as the bldc_interface is expecting simple function to be
// passed as a parameter. Passing a class function memebr is not permitted.

void sendUartData(unsigned char *data, unsigned int len)
{
  // Serial.println("Package Sent to VESC: ");
  // Serial.print(data[0]);
  // Serial.print(" | ");
  // Serial.print(data[1]);
  // Serial.print(" | ");
  // Serial.print(data[2]);
  // Serial.print(" | ");
  // Serial.print(data[3]);
  // Serial.print(" | ");
  // Serial.print(data[4]);
  // Serial.print(" | ");
  // Serial.print(data[5]);
  // Serial.print(" | ");
  // Serial.print(data[6]);
  // Serial.print(" | ");
  // Serial.print(data[7]);
  // Serial.print(" | ");
  // Serial.print(data[8]);
  // Serial.print(" | ");
  // Serial.print(data[9]);
  // Serial.print(" | ");
  // Serial.print(data[10]);
  // Serial.print(" | ");
  // Serial.print(data[11]);
  // Serial.print(" | ");
  // Serial.print(data[12]);
  // Serial.println(" | ");
  // Serial.write(data, len);

  // Writing data to serial.
  Serial.write(data, len);
}


void printValues(mc_values *val)
{
  // TODO:: See what we are doing with all of this data
  // Example: use have it be used by a logger.

  Serial.print("Input voltage: ");
  Serial.println(val->v_in);
  Serial.print("Temp: ");
  Serial.println(val->temp_pcb);
  Serial.print("Current motor: ");
  Serial.println(val->current_motor);
  Serial.print("Current in: ");
  Serial.println(val->current_in);
  Serial.print("Current motor: ");
  Serial.println(val->current_motor);
  Serial.print("Current rpm: ");
  Serial.println(val->current_motor);
  Serial.print("Duty cycle: ");
  Serial.println(val->current_motor);
  Serial.print("Duty cycle: ");
  Serial.println(val->duty_now * 100.0);
  Serial.print("Ah Drawn: ");
  Serial.println(val->amp_hours);
  Serial.print("Ah Regen: ");
  Serial.println(val->amp_hours_charged);
  Serial.print("Wh Drawn: ");
  Serial.println(val->watt_hours);
  Serial.print("Wh Regen: ");
  Serial.println(val->watt_hours_charged);
  Serial.print("Tacho: ");
  Serial.println(val->tachometer);
  Serial.print("Tacho ABS: ");
  Serial.println(val->tachometer_abs);
}

void updateMotorController(MotorController* motorController)
{
  motorController->update();
}

#endif
