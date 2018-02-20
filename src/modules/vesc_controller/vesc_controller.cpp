#include <HardwareSerial.h>
#include <Arduino.h>
#include <algorithm>
#include <functional>

#include "./vesc_datatypes.h"

#include "./vesc_controller.h"
#include "./vesc_controller_wheel_decorator.h"
#include "./vesc_interface.h"
#include "../../configuration/default/configuration.h"
#include "../base/limit_module.h"
#include "../../utility/tools.h"

using namespace std;
using namespace std::placeholders;

static void vescTimerTask(void * parameter)
{
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
  for(;;){
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
    {
      Configurator::Instance().initializeSerial();
        pVescInterface = new Vesc_interface(&Serial, this);
    }
    else
    {
      pinMode(16, OUTPUT);
      pinMode(17, OUTPUT);
      HardwareSerial * vescSerial = new HardwareSerial(2);
      vescSerial -> begin(BAUDRATE);
      while (!vescSerial);
        pVescInterface = new Vesc_interface(vescSerial, this);
    }

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
      1,                        // priority of the task
      NULL);

//  xTaskCreatePinnedToCore(vescTimerTask, "vescTimerTask", 4096, NULL, 1, NULL, xPortGetCoreID());
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
        pVescInterface -> loop();

        //Copy all inputs to the individual powered wheels
        float minRpm = 10000;
        for (byte i=0; i<wheelDecorators.size(); i++)
        {
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
      pVescInterface -> set_forward_can(wheelDecorators[i] -> getCanId());
      pVescInterface -> set_current_brake(0);
  }
}

void Vesc_controller::setValues(mc_values * val)
{
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("********************** setValues **********************"));
  mMotorValues.temp_mos1 = val->temp_mos1;
  mMotorValues.temp_mos2 = val->temp_mos2;
  mMotorValues.temp_mos3 = val->temp_mos3;
  mMotorValues.temp_mos4 = val->temp_mos4;
  mMotorValues.temp_mos5 = val->temp_mos5;
  mMotorValues.temp_mos6 = val->temp_mos6;
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

  if (wheelDecorators.size()>0)
  {
    wheelDecorators[mVescArrayIndex] -> setVescValues(mMotorValues);
    String idx = String(mVescArrayIndex);
    mFMV -> sensors().add("v_in_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().v_in);
    mFMV -> sensors().add("t_pcb_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().temp_pcb);
    mFMV -> sensors().add("rpm_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().rpm);
    mFMV -> sensors().add("c_mot_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().current_motor);
    mFMV -> sensors().add("c_in_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().current_in);
    mFMV -> sensors().add("duty_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().duty_now);
    mFMV -> sensors().add("a_hours_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().amp_hours);
    mFMV -> sensors().add("a_charged_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().amp_hours_charged);
    mFMV -> sensors().add("w_hours_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().watt_hours);
    mFMV -> sensors().add("w_charged_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().watt_hours_charged);
    mFMV -> sensors().add("tacho_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().tachometer);
    mFMV -> sensors().add("techo_abs_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().tachometer_abs);
    mFMV -> sensors().add("fault_" + idx, wheelDecorators[mVescArrayIndex] -> getVescValues().fault_code);

    if (mVescArrayIndex+1 >= wheelDecorators.size())
      mVescArrayIndex = 0;
    else
      mVescArrayIndex++;
  }
}

void Vesc_controller::print(char *str){
}

void Vesc_controller::setFirmware(int major, int minor){
}

void Vesc_controller::setRotorPos(float pos){
}

void Vesc_controller::setMotorConfiguration(mc_configuration *conf){
}

void Vesc_controller::setAppConfiguration(app_configuration *conf){
}

void Vesc_controller::setDetect(float cycle_int_limit, float coupling_k, const signed char *hall_table, signed char hall_res){
}

void Vesc_controller::setPPM(float val, float ms){
}

void Vesc_controller::setADC(float val, float voltage){
}

void Vesc_controller::setChuk(float val){
}

void Vesc_controller::motorConfigurationRecieved(){
}

void Vesc_controller::appConfigurationRecieved(){
}

void Vesc_controller::timerRun()
{
	Vesc_interface::timerTimeouts();
}

void Vesc_controller::setCurrent()
{
  //Here we read the values on the individual wheels and send to the responsible vesc
  for (byte i=0; i<wheelDecorators.size(); i++)
  {
    if (wheelDecorators[i] -> getWheelControl().getPower() > 0)
    {
      float powerCurrent = Tools::mapFloat(wheelDecorators[i] -> getWheelControl().getPower(), 0, 100, 0.0, float(mMaxPowerCurrent));
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setCurrentPower: "), String(powerCurrent));
      pVescInterface -> set_forward_can(wheelDecorators[i] -> getCanId());
      pVescInterface -> set_current(powerCurrent);
    }
    else if (wheelDecorators[i] -> getWheelControl().getBrake() > 0)
    {
      float brakeCurrent = Tools::mapFloat(wheelDecorators[i] -> getWheelControl().getBrake(), 0, 100, 0.0, float(mMaxBrakeCurrent));
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setCurrentBrake: "), String(brakeCurrent));
      pVescInterface -> set_forward_can(wheelDecorators[i] -> getCanId());
      pVescInterface -> set_current_brake(brakeCurrent);
    }
    else
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setCurrentNeutral: "), String(0));
      pVescInterface -> set_forward_can(wheelDecorators[i] -> getCanId());
      pVescInterface -> set_current_brake(0);
    }
  }
}

void Vesc_controller::setRpm(int rpm)
{
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setRpm: "), String(rpm));
  for (int i=0; i<wheelDecorators.size(); i++)
  {
      pVescInterface -> set_forward_can(wheelDecorators[i] -> getCanId());
      pVescInterface -> set_rpm(rpm);
  }
}

void Vesc_controller::getValues()
{
  if (wheelDecorators.size() > 0)
  {
    Logger::Instance().write(LogLevel::DEBUG, "Vesc_controller::getValues: " + String(mVescArrayIndex));
    pVescInterface -> set_forward_can(wheelDecorators[mVescArrayIndex] -> getCanId());
    pVescInterface -> get_values();
  }
}
