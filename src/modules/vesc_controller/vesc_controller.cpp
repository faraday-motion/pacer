#include <HardwareSerial.h>
#include <Arduino.h>
#include <algorithm>
#include <functional>

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
#include "../base/limit_module.h"
#include "../../utility/tools.h"

using namespace std;
using namespace std::placeholders;

static void vescTimerTask(void * parameter)
{
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
  for(;;){
  //  bldc_interface_uart_run_timer();
    Vesc_controller::timerRun();
    vTaskDelay(xDelay);
  }
  vTaskDelete( NULL );
}

void Vesc_controller::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Vesc_controller"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("DefaultSerial: "), String(mVescDefaultSerial));
    if (mVescDefaultSerial)
      Configurator::Instance().initializeSerial();
    else
    {
      pinMode(16, OUTPUT);
      pinMode(17, OUTPUT);
      pVescSerial = new HardwareSerial(2);
      pVescSerial -> begin(BAUDRATE);
      while (!pVescSerial);
    }
    serialInit();

    //Configure the vesc's
    std::vector<Wheel*> wheelArray = mFMV -> getWheelValues();
    for (byte i=0; i<wheelArray.size(); i++)
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
      2,                        // priority of the task
      NULL);
//  xTaskCreatePinnedToCore(additionalTask, "additionalTask", 4096, NULL, 1, NULL, 0);
}

void Vesc_controller::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check()) {
      Modulebase* mb = mFMV -> modules().getEnabled(Roles::LIMIT_MODULE);
      if (mb != nullptr)
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::loop"));
        Limit_module* ic = static_cast<Limit_module*>(mb);
        mInputControl = Vehiclecontrol(ic -> getOutputControl());
        serialRead();

        //Copy all inputs to the individual powered wheels
        float minRpm = 10000;
        for (byte i=0; i<wheelDecorators.size(); i++)
        {
          if (i == mVescArrayIndex)
          {
            String idx = String(i);
            wheelDecorators[i] -> setVescValues(mMotorValues);
            mFMV -> sensors().add("vesc_" + idx + "_v_in", wheelDecorators[i] -> getVescValues().v_in);
            mFMV -> sensors().add("vesc_" + idx + "_t_pcb", wheelDecorators[i] -> getVescValues().temp_pcb);
            mFMV -> sensors().add("vesc_" + idx + "_rpm", wheelDecorators[i] -> getVescValues().rpm);
            mFMV -> sensors().add("vesc_" + idx + "_c_mot", wheelDecorators[i] -> getVescValues().current_motor);
            mFMV -> sensors().add("vesc_" + idx + "_c_in", wheelDecorators[i] -> getVescValues().current_in);
            mFMV -> sensors().add("vesc_" + idx + "_duty", wheelDecorators[i] -> getVescValues().duty_now);
            mFMV -> sensors().add("vesc_" + idx + "_a_hours", wheelDecorators[i] -> getVescValues().amp_hours);
            mFMV -> sensors().add("vesc_" + idx + "_a_charged", wheelDecorators[i] -> getVescValues().amp_hours_charged);
            mFMV -> sensors().add("vesc_" + idx + "_w_hours", wheelDecorators[i] -> getVescValues().watt_hours);
            mFMV -> sensors().add("vesc_" + idx + "_w_charged", wheelDecorators[i] -> getVescValues().watt_hours_charged);
            mFMV -> sensors().add("vesc_" + idx + "_tacho", wheelDecorators[i] -> getVescValues().tachometer);
            mFMV -> sensors().add("vesc_" + idx + "_techo_abs", wheelDecorators[i] -> getVescValues().tachometer_abs);
            mFMV -> sensors().add("vesc_" + idx + "_fault", wheelDecorators[i] -> getVescValues().fault_code);
          }

          minRpm = std::min(minRpm, wheelDecorators[i] -> getVescValues().rpm);
          //Set the wheel control to the input controls
          wheelDecorators[i] -> setWheelControl(mInputControl);
        }

        //Set master
        for (byte i=0; i<wheelDecorators.size(); i++)
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
  for (byte i=0; i<wheelDecorators.size(); i++)
  {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::onDisable"));
      bldc_interface_set_forward_can(wheelDecorators[i] -> getCanId());
      bldc_interface_set_current_brake(0);
  }
}

void Vesc_controller::serialWrite(unsigned char *data, unsigned int len)
{
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("serialWrite: "), String(len));
  if (mVescDefaultSerial)
    Serial.write(data, len);
  else
    pVescSerial -> write(data, len);
}

void Vesc_controller::setValues(mc_values * val)
{
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("setValues: vin-"), String(val->v_in));
  // Input Voltage
  mMotorValues.v_in = val->v_in;
  // VESC PCB Temperature
  mMotorValues.temp_pcb = val->temp_pcb;
  // Current of the Motor
  mMotorValues.current_motor = val->current_motor;
  //Current input
  mMotorValues.current_in = val->current_in;
  //RPM
  mMotorValues.rpm = val->rpm;
  //Duty Cycle
  mMotorValues.duty_now = val->duty_now * 100.0;
  //Ah Drawn
  mMotorValues.amp_hours = val->amp_hours;
  //Ah Regenerate
  mMotorValues.amp_hours_charged = val->amp_hours_charged;
  //Wh Drawn
  mMotorValues.watt_hours = val->watt_hours;
  //Wh Regen
  mMotorValues.watt_hours_charged = val->watt_hours_charged;
  //Tacho
  mMotorValues.tachometer = val->tachometer;
  //Tacho ABS
  mMotorValues.tachometer_abs = val->tachometer_abs;
  //Fault Code
  mMotorValues.fault_code = val->fault_code;
}

void Vesc_controller::serialInit()
{
	bldc_interface_uart_init(serialWrite);
	bldc_interface_set_rx_value_func(setValues);
  //bldc_interface_uart_init(std::bind(&Vesc_controller::serialWrite, this, _1, _2));
	//bldc_interface_set_rx_value_func(std::bind(&Vesc_controller::setValues, this, _1));
}

void Vesc_controller::serialRead()
{
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("serialRead: "), String(mVescDefaultSerial));
  if (mVescDefaultSerial)
  {
    while (Serial.available() > 0)
      bldc_interface_uart_process_byte(Serial.read());
  }
  else
  {
    while (pVescSerial -> available() > 0)
      bldc_interface_uart_process_byte(pVescSerial -> read());
  }
}

void Vesc_controller::timerRun()
{
	bldc_interface_uart_run_timer();
}

void Vesc_controller::setCurrent()
{
  //Here we read the values on the individual wheels and send to the responsible vesc
  /*
  for (byte i=0; i<wheelDecorators.size(); i++)
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
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setCurrentNeutral: "), String(0));
      bldc_interface_set_forward_can(wheelDecorators[i] -> getCanId());
      bldc_interface_set_current_brake(0);
    }
  }
  */
  //bldc_interface_set_forward_can(wheelDecorators[i] -> getCanId());
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setCurrentBrakeNeutral: "), String(0));
  bldc_interface_set_current_brake(0);
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
    Logger::Instance().write(LogLevel::DEBUG, "Vesc_controller::getValues: " + String(wheelDecorators[mVescArrayIndex] -> getCanId()));
    bldc_interface_set_forward_can(wheelDecorators[mVescArrayIndex] -> getCanId());
    bldc_interface_get_values();
    if (mVescArrayIndex+1 == wheelDecorators.size())
      mVescArrayIndex = 0;
    else
      mVescArrayIndex++;
  }
}
