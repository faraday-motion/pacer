#include <Arduino.h> // used for Serial.
#include "VescParams.h"

// Global static pointer used to ensur ea sigle instance of this class.
VescParams* VescParams::m_pInstance = nullptr;


/**
    This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

VescParams* VescParams::Instance()
{
  if(!m_pInstance)
  {
    m_pInstance = new VescParams;
  }

  return m_pInstance;
}


void VescParams::setMotorValues(mc_values *val)
{
    // Input Voltage
    motorValues.v_in = val->v_in;
    // VESC PCB Temperature
    motorValues.temp_pcb = val->temp_pcb;
    // Current of the Motor
    motorValues.current_motor = val->current_motor;
    //Current input
    motorValues.current_in = val->current_in;
    //RPM
    motorValues.rpm = val->rpm;
    //Duty Cycle
    motorValues.duty_now = val->duty_now * 100.0; // Why are we multiplying by 100?
    //Ah Drawn
    motorValues.amp_hours = val->amp_hours;
    //Ah Regenerate
    motorValues.amp_hours_charged = val->amp_hours_charged;
    //Wh Drawn
    motorValues.watt_hours = val->watt_hours;
    //Wh Regen
    motorValues.watt_hours_charged = val->watt_hours_charged;
    //Tacho
    motorValues.tachometer = val->tachometer;
    //Tacho ABS
    motorValues.tachometer_abs = val->tachometer_abs;
    //Fault Code
    motorValues.fault_code = val->fault_code;
}



// Debug

void VescParams::printMotorValues()
{
  Serial.print("Input voltage: ");
  Serial.println(motorValues.v_in);
  Serial.print("Temp: ");
  Serial.println(motorValues.temp_pcb);
  Serial.print("Current motor: ");
  Serial.println(motorValues.current_motor);
  Serial.print("Current in: ");
  Serial.println(motorValues.current_in);
  Serial.print("Current rpm: ");
  Serial.println(motorValues.current_motor);
  Serial.print("Duty cycle: ");
  Serial.println(motorValues.current_motor);
  Serial.print("Duty cycle: ");
  Serial.println(motorValues.duty_now * 100.0);
  Serial.print("Ah Drawn: ");
  Serial.println(motorValues.amp_hours);
  Serial.print("Ah Regen: ");
  Serial.println(motorValues.amp_hours_charged);
  Serial.print("Wh Drawn: ");
  Serial.println(motorValues.watt_hours);
  Serial.print("Wh Regen: ");
  Serial.println(motorValues.watt_hours_charged);
  Serial.print("Tacho: ");
  Serial.println(motorValues.tachometer);
  Serial.print("Tacho ABS: ");
  Serial.println(motorValues.tachometer_abs);
  Serial.print("Fault Code");
  Serial.println(motorValues.fault_code);
}
