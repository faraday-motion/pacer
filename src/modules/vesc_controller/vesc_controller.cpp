#include <HardwareSerial.h>
#include <Arduino.h>
#include <algorithm>

#include "packet.h"
#include "packet.c"
#include "buffer.h"
#include "buffer.c"
#include "crc.h"
#include "crc.c"
#include "bldc_interface.h"
#include "bldc_interface.c"
#include "bldc_interface_uart.h"
#include "bldc_interface_uart.c"
#include "datatypes.h"

#include "./vesc_controller.h"
#include "./vesc_controller_wheel_decorator.h"
#include "../../configuration/default/configuration.h"
#include "../base/input_limit.h"
#include "../../utility/tools.h"

static mc_values motorValues;
static byte vescArrayIndex;

static void vescTimerTask( void * parameter )
{
  for(;;){
    Vesc_controller::timerRun();
    delay(1);
  }
  vTaskDelete( NULL );
}

void Vesc_controller::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Vesc_controller"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    if (sVescDefaultSerial)
      Configurator::Instance().initializeSerial();
    else
    {
      sVescSerial = new HardwareSerial(2);
      sVescSerial -> begin(BAUDRATE);
      while (!sVescSerial);
    }
    serialInit();

    //Configure the vesc's
    std::vector<Wheel*> wheelArray = mFMV -> getWheelValues();
    for (int i=0; i<wheelArray.size(); i++)
    {
      if (wheelArray[i] -> isElectric())
        wheelDecorators.push_back(new Vesc_controller_wheel_decorator(wheelArray[i]));
    }

    createTimerTask();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Vesc_controller"));
    mIsSetup = true;
  }
}

void Vesc_controller::createTimerTask()
{
  xTaskCreate(vescTimerTask,           // Task function.
      "vescTimerTask",        // name of task.
      4096,                    // Stack size of task
      NULL,                     // parameter of the task
      1,                        // priority of the task
      NULL);
//  xTaskCreatePinnedToCore(additionalTask, "additionalTask", 4096, NULL, 1, NULL, 0);
}

void Vesc_controller::loop()
{
  if (enabled())
  {
    return;
    if (mSimpleTimer.check()) {
      Modulebase* mb = mFMV -> modules().getEnabled(Roles::INPUT_LIMIT);
      if (mb != nullptr)
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::loop"));
        Input_limit* ic = static_cast<Input_limit*>(mb);
        mInputControl = Vehiclecontrol(ic -> getOutputControl());
        serialRead();

        wheelDecorators[vescArrayIndex] -> setVescValues(motorValues);
        //Copy all inputs to the individual powered wheels
        float minRpm = 10000;
        float currentMotor = 0;
        float currentIn = 0;
        for (int i=0; i<wheelDecorators.size(); i++)
        {
          minRpm = std::min(minRpm, wheelDecorators[i] -> getVescValues().rpm);
          currentMotor = wheelDecorators[i] -> getVescValues().current_motor;
          currentIn = wheelDecorators[i] -> getVescValues().current_in;

          //Set the wheel control to the input controls
          wheelDecorators[i] -> setWheelControl(mInputControl);
        }
        mFMV -> sensors().add("minRpm", minRpm);
        mFMV -> sensors().add("currentMotor", currentMotor);
        mFMV -> sensors().add("currentIn", currentIn);

        //Set master
        for (int i=0; i<wheelDecorators.size(); i++)
        {
          if (minRpm == wheelDecorators[i] -> getVescValues().rpm)
          {
            wheelDecorators[i] -> setMaster(true);
            break;
          }
        }
        //TODO
        //Add a section where we do analytics on the individual wheels in order to make advanced driving possible
        //We can add rpm control for cruisecontrol
        setCurrent();
        getValues();
      }
    }
  }
}

void Vesc_controller::command(byte command)
{

}

void Vesc_controller::onDisable()
{
  for (int i=0; i<wheelDecorators.size(); i++)
  {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::onDisable"));
      bldc_interface_set_forward_can(wheelDecorators[i] -> getCanId());
      bldc_interface_set_current_brake(0);
  }
}

void serialWrite(unsigned char *data, unsigned int len)
{
  if (sVescDefaultSerial)
    Serial.write(data, len);
  else
    sVescSerial -> write(data, len);
}

void setValues(mc_values *val)
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
  motorValues.duty_now = val->duty_now * 100.0;
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

void Vesc_controller::serialInit()
{
	bldc_interface_uart_init(serialWrite);
	bldc_interface_set_rx_value_func(setValues);
}

void Vesc_controller::serialRead()
{
  if (sVescDefaultSerial)
  {
    while (Serial.available() > 0)
      bldc_interface_uart_process_byte(Serial.read());
  }
  else
  {
    while (sVescSerial -> available() > 0)
      bldc_interface_uart_process_byte(sVescSerial -> read());
  }
}

void Vesc_controller::timerRun()
{
	bldc_interface_uart_run_timer();
}

void Vesc_controller::setCurrent()
{
  //Here we read the values on the individual wheels and send to the responsible vesc
  for (int i=0; i<wheelDecorators.size(); i++)
  {
    if (wheelDecorators[i] -> getWheelControl().getPower() > 0)
    {
      float powerCurrent = Tools::mapFloat(wheelDecorators[i] -> getWheelControl().getPower(), 0, 100, 0.0, float(mMaxPowerCurrent));
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setCurrentPower: "), String(powerCurrent));
      bldc_interface_set_forward_can(wheelDecorators[i] -> getCanId());
      bldc_interface_set_current(powerCurrent);
    }
    else if (wheelDecorators[i] -> getWheelControl().getBrake() > 0)
    {
      float brakeCurrent = Tools::mapFloat(wheelDecorators[i] -> getWheelControl().getBrake(), 0, 100, 0.0, float(mMaxBrakeCurrent));
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setCurrentBrake: "), String(brakeCurrent));
      bldc_interface_set_forward_can(wheelDecorators[i] -> getCanId());
      bldc_interface_set_current_brake(brakeCurrent);
    }
    else
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setCurrentBrake: "), String(0));
      bldc_interface_set_forward_can(wheelDecorators[i] -> getCanId());
      bldc_interface_set_current_brake(0);
    }
  }
}

void Vesc_controller::setRpm(int rpm)
{
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setRpm: "), String(rpm));
  for (int i=0; i<wheelDecorators.size(); i++)
  {
      bldc_interface_set_forward_can(wheelDecorators[i] -> getCanId());
      bldc_interface_set_rpm(rpm);
  }
}

void Vesc_controller::getValues()
{
  if (wheelDecorators.size() > 0)
  {
    Logger::Instance().write(LogLevel::DEBUG, "Vesc_controller::getValues: " + String(wheelDecorators[vescArrayIndex] -> getCanId()));
    bldc_interface_set_forward_can(wheelDecorators[vescArrayIndex] -> getCanId());
    bldc_interface_get_values();
    if (vescArrayIndex+1 == wheelDecorators.size())
      vescArrayIndex = 0;
    else
      vescArrayIndex++;
  }
}
