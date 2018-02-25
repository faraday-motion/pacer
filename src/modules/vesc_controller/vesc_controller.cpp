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
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("DefaultSerial: "), String(mVescDefaultSerial));
    mDriveMode = Commands::CURRENT_MODE;
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
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    mIsSetup = true;
  }
}

void Vesc_controller::createTimerTask()
{
  xTaskCreate(vescTimerTask, "vescTimerTask", 4096, NULL, 1, NULL);
//  xTaskCreatePinnedToCore(vescTimerTask, "vescTimerTask", 4096, NULL, 1, NULL, xPortGetCoreID());
}

void Vesc_controller::loop()
{
  if (enabled())
  {
    pVescInterface -> loop();
    if (mSimpleTimer.check()) {
      Modulebase* mb = mFMV -> modules().getEnabledByRole(Roles::LIMIT_MODULE);
      if (mb != nullptr)
      {
        Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
        Limit_module* ic = static_cast<Limit_module*>(mb);
        mInputControl = Vehiclecontrol(ic -> getOutputControl());
        getValues();
/*
        //Not working currently
        //Get the motor configuration
        Logger::Instance().write(LogLevel::INFO, FPSTR("Requesting motor configuration"));
        pVescInterface -> set_forward_can(-1);
        pVescInterface -> get_mcconf();
*/
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
        if (mDriveMode == Commands::CURRENT_MODE)
          setCurrent();
        else if (mDriveMode == Commands::RPM_MODE)
          setRpm();
      }
    }
  }
}

void Vesc_controller::command(byte command)
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, "recievecommand: " + (String)command);
    Commands comm = static_cast<Commands>(command);
    if (comm == Commands::CURRENT_MODE)
    {
      mDriveMode = Commands::CURRENT_MODE;
    }
    else if (comm == Commands::RPM_MODE)
    {
      mDriveMode = Commands::RPM_MODE;
    }
  }
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
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setValues "));
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
    mFMV -> sensors().add("vesc_" + idx + "_v_in", wheelDecorators[mVescArrayIndex] -> getVescValues().v_in);
    mFMV -> sensors().add("vesc_" + idx + "_t_pcb", wheelDecorators[mVescArrayIndex] -> getVescValues().temp_pcb);
    mFMV -> sensors().add("vesc_" + idx + "_rpm", wheelDecorators[mVescArrayIndex] -> getVescValues().rpm);
    mFMV -> sensors().add("vesc_" + idx + "_c_mot", wheelDecorators[mVescArrayIndex] -> getVescValues().current_motor);
    mFMV -> sensors().add("vesc_" + idx + "_c_in", wheelDecorators[mVescArrayIndex] -> getVescValues().current_in);
    mFMV -> sensors().add("vesc_" + idx + "_duty", wheelDecorators[mVescArrayIndex] -> getVescValues().duty_now);
    mFMV -> sensors().add("vesc_" + idx + "_a_hours", wheelDecorators[mVescArrayIndex] -> getVescValues().amp_hours);
    mFMV -> sensors().add("vesc_" + idx + "_a_charged", wheelDecorators[mVescArrayIndex] -> getVescValues().amp_hours_charged);
    mFMV -> sensors().add("vesc_" + idx + "_w_hours", wheelDecorators[mVescArrayIndex] -> getVescValues().watt_hours);
    mFMV -> sensors().add("vesc_" + idx + "_w_charged", wheelDecorators[mVescArrayIndex] -> getVescValues().watt_hours_charged);
    mFMV -> sensors().add("vesc_" + idx + "_tacho", wheelDecorators[mVescArrayIndex] -> getVescValues().tachometer);
    mFMV -> sensors().add("vesc_" + idx + "_techo_abs", wheelDecorators[mVescArrayIndex] -> getVescValues().tachometer_abs);
    mFMV -> sensors().add("vesc_" + idx + "_fault", wheelDecorators[mVescArrayIndex] -> getVescValues().fault_code);

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
  mHasMCConfiguration = true;
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setMotorConfiguration "));
  mc_configuration actual ;
  actual.pwm_mode = conf -> pwm_mode;
	actual.comm_mode = conf -> comm_mode;
	actual.motor_type = conf -> motor_type;
	actual.sensor_mode = conf -> sensor_mode;
	// Limits
	actual.l_current_max = conf -> l_current_max;
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::l_current_max "), String(actual.l_current_max));
	actual.l_current_min = conf -> l_current_min;
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::l_current_min "), String(actual.l_current_min));
	actual.l_in_current_max = conf -> l_in_current_max;
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::l_in_current_max "), String(actual.l_in_current_max));
	actual.l_in_current_min = conf -> l_in_current_min;
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::l_in_current_min "), String(actual.l_in_current_min));
	actual.l_abs_current_max = conf -> l_abs_current_max;
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::l_abs_current_max "), String(actual.l_abs_current_max));
	actual.l_min_erpm = conf -> l_min_erpm;
	actual.l_max_erpm = conf -> l_max_erpm;
	actual.l_max_erpm_fbrake = conf -> l_max_erpm_fbrake;
	actual.l_max_erpm_fbrake_cc = conf -> l_max_erpm_fbrake_cc;
	actual.l_min_vin = conf -> l_min_vin;
	actual.l_max_vin = conf -> l_max_vin;
	actual.l_battery_cut_start = conf -> l_battery_cut_start;
	actual.l_battery_cut_end = conf -> l_battery_cut_end;
	actual.l_slow_abs_current = conf -> l_slow_abs_current;
	actual.l_rpm_lim_neg_torque = conf -> l_rpm_lim_neg_torque;
	actual.l_temp_fet_start = conf -> l_temp_fet_start;
	actual.l_temp_fet_end = conf -> l_temp_fet_end;
	actual.l_temp_motor_start = conf -> l_temp_motor_start;
	actual.l_temp_motor_end = conf -> l_temp_motor_end;
	actual.l_min_duty = conf -> l_min_duty;
	actual.l_max_duty = conf -> l_max_duty;
	// Overridden limits (Computed during runtime)
	actual.lo_current_max = conf -> lo_current_max;
	actual.lo_current_min = conf -> lo_current_min;
	actual.lo_in_current_max = conf -> lo_in_current_max;
	actual.lo_in_current_min = conf -> lo_in_current_min;
	// Sensorless
	actual.sl_min_erpm = conf -> sl_min_erpm;
	actual.sl_min_erpm_cycle_int_limit = conf -> sl_min_erpm_cycle_int_limit;
	actual.sl_max_fullbreak_current_dir_change = conf -> sl_max_fullbreak_current_dir_change;
	actual.sl_cycle_int_limit = conf -> sl_cycle_int_limit;
	actual.sl_phase_advance_at_br = conf -> sl_phase_advance_at_br;
	actual.sl_cycle_int_rpm_br = conf -> sl_cycle_int_rpm_br;
	actual.sl_bemf_coupling_k = conf -> sl_bemf_coupling_k;
	// Hall sensor
	//actual.hall_table[8];
	actual.hall_sl_erpm = conf -> hall_sl_erpm;
	// FOC
	actual.foc_current_kp = conf -> foc_current_kp;
	actual.foc_current_ki = conf -> foc_current_ki;
	actual.foc_f_sw = conf -> foc_f_sw;
	actual.foc_dt_us = conf -> foc_dt_us;
	actual.foc_encoder_offset = conf -> foc_encoder_offset;
	actual.foc_encoder_inverted = conf -> foc_encoder_inverted;
	actual.foc_encoder_ratio = conf -> foc_encoder_ratio;
	actual.foc_motor_l = conf -> foc_motor_l;
	actual.foc_motor_r = conf -> foc_motor_r;
	actual.foc_motor_flux_linkage = conf -> foc_motor_flux_linkage;
	actual.foc_observer_gain = conf -> foc_observer_gain;
	actual.foc_pll_kp = conf -> foc_pll_kp;
	actual.foc_pll_ki = conf -> foc_pll_ki;
	actual.foc_duty_dowmramp_kp = conf -> foc_duty_dowmramp_kp;
	actual.foc_duty_dowmramp_ki = conf -> foc_duty_dowmramp_ki;
	actual.foc_openloop_rpm = conf -> foc_openloop_rpm;
	actual.foc_sl_openloop_hyst = conf -> foc_sl_openloop_hyst;
	actual.foc_sl_openloop_time = conf -> foc_sl_openloop_time;
	actual.foc_sl_d_current_duty = conf -> foc_sl_d_current_duty;
	actual.foc_sl_d_current_factor = conf -> foc_sl_d_current_factor;
	actual.foc_sensor_mode = conf -> foc_sensor_mode;
	//actual.ufoc_hall_table[8];
	actual.foc_hall_sl_erpm = conf -> foc_hall_sl_erpm;
	// Speed PID
	actual.s_pid_kp = conf -> s_pid_kp;
	actual.s_pid_ki = conf -> s_pid_ki;
	actual.s_pid_kd = conf -> s_pid_kd;
	actual.s_pid_min_erpm = conf -> s_pid_min_erpm;
	// Pos PID
	actual.p_pid_kp = conf -> p_pid_kp;
	actual.p_pid_ki = conf -> p_pid_ki;
	actual.p_pid_kd = conf -> p_pid_kd;
	actual.p_pid_ang_div = conf -> p_pid_ang_div;
	// Current controller
	actual.cc_startup_boost_duty = conf -> cc_startup_boost_duty;
	actual.cc_min_current = conf -> cc_min_current;
	actual.cc_gain = conf -> cc_gain;
	actual.cc_ramp_step_max = conf -> cc_ramp_step_max;
	// Misc
	actual.m_fault_stop_time_ms = conf -> m_fault_stop_time_ms;
	actual.m_duty_ramp_step = conf -> m_duty_ramp_step;
	actual.m_duty_ramp_step_rpm_lim = conf -> m_duty_ramp_step_rpm_lim;
	actual.m_current_backoff_gain = conf -> m_current_backoff_gain;
	actual.m_encoder_counts = conf -> m_encoder_counts;
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

void Vesc_controller::setRpm()
{
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_controller::setRpm "));
  for (int i=0; i<wheelDecorators.size(); i++)
  {
      int rpm = map(wheelDecorators[i] -> getWheelControl().getBrake(), 0, 100, 0, mMaxRpm);
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
