#ifndef DEFAULT_CONFIG_H
#define DEFAULT_CONFIG_H

#include <vector>
#include <Arduino.h>
#include "../base/default_configbase.h"
#include "./configuration.h"
#include "../configurator.h"
#include "../../enums/loglevel.h"
#include "../../modules/modules.hpp"
#include "../wheel_config.h"

class Default_config : public Default_configbase{
private:

public:

  Default_config() : Default_configbase() {

  }

  virtual void addConfigs()
  {
      byte id = 0;

      //0
      Serial_log_config * cfg0 = new Serial_log_config(id++);
      cfg0 -> logLevel = LogLevel::INFO;
      cfg0 -> enabled = false;
      Configurator::Instance().addConfig(cfg0);

/*
      //The drivelog needs refactoring to tasks as it performs bad and stalls normal execution
      Spiffs_log_config * spiff_log = new Spiffs_log_config(id++);
      spiff_log -> logLevel = LogLevel::WARNING;
      spiff_log -> enabled = true;
      Configurator::Instance().addConfig(spiff_log);
*/
      //15
      Vesc_controller_config * cfg15 = new Vesc_controller_config(id++);
      cfg15 -> defaultSerial = true;
      cfg15 -> maxRpm = 10000;
      cfg15 -> maxPowerCurrent = 30;
      cfg15 -> maxBrakeCurrent = 40;
      cfg15 -> enabled = true;
      cfg15 -> driveMode = 1; //0=Current 1=RPM
      Configurator::Instance().addConfig(cfg15);

/*
      //The drivelog needs refactoring to tasks as it performs bad and stalls normal execution
      Spiffs_drivelog_config * cfg23 = new Spiffs_drivelog_config(id++);
      cfg23 -> enabled = false;
      Configurator::Instance().addConfig(cfg23);
*/

      //1
      Wheel_config * cfg4 = new Wheel_config(id++);
      cfg4 -> index = 0;
      cfg4 -> isElectric = true;
      cfg4 -> canSteer = false;
      cfg4 -> isInverse = false;
      Configurator::Instance().addConfig(cfg4);
/*
      //2
      Wheel_config * cfg5 = new Wheel_config(id++);
      cfg5 -> index = 1;
      cfg5 -> isElectric = false;
      cfg5 -> canSteer = false;
      cfg5 -> isInverse = false;
      Configurator::Instance().addConfig(cfg5);
*/
      //3
      Wheel_config * cfg6 = new Wheel_config(id++);
      cfg6 -> index = 1;
      cfg6 -> isElectric = false;
      cfg6 -> canSteer = true;
      cfg6 -> isInverse = true;
      Configurator::Instance().addConfig(cfg6);

      //7
      Wheel_config * cfg7 = new Wheel_config(id++);
      cfg7 -> index = 2;
      cfg7 -> isElectric = false;
      cfg7 -> canSteer = true;
      cfg7 -> isInverse = true;
      Configurator::Instance().addConfig(cfg7);

      //13
      //If the wifi is configured late it gives crashes for some reason
      Wifi_connection_config * cfg13 = new Wifi_connection_config(id++);
      cfg13 -> enabled = true;
      Configurator::Instance().addConfig(cfg13);

      /*
      //14
      Wifi_simple_control_config * cfg14 = new Wifi_simple_control_config(id++);
      cfg14 -> enabled = false;
      Configurator::Instance().addConfig(cfg14);
      */

      //1
      Boolean_sensor_config * cfg_dead = new Boolean_sensor_config(id++);
      cfg_dead -> pin = PIN_DEAD_MAN_SWITCH;
      cfg_dead -> interval = 100;
      cfg_dead -> critical = 200;
      cfg_dead -> invert = true;
      cfg_dead -> sensorName = "dead";
      Configurator::Instance().addConfig(cfg_dead);

      Boolean_sensor_config * cfg_joy_switch = new Boolean_sensor_config(id++);
      cfg_joy_switch -> pin = PIN_JOYSTICK_ENABLE_SWITCH;
      cfg_joy_switch -> interval = 100;
      cfg_joy_switch -> critical = 200;
      cfg_joy_switch -> invert = true;
      cfg_joy_switch -> sensorName = "joy_enable";
      Configurator::Instance().addConfig(cfg_joy_switch);

      //8
      Analog_sensor_config * cfg8 = new Analog_sensor_config(id++);
      cfg8 -> pin = PIN_JOYSTICK_CONTROL_Y;
      cfg8 -> interval = 50;
      cfg8 -> critical = 100;
      cfg8 -> sensorName = "joy_y";
      cfg8 -> enabled = true;
      Configurator::Instance().addConfig(cfg8);

      Analog_sensor_config * cfg81 = new Analog_sensor_config(id++);
      cfg81 -> pin = PIN_JOYSTICK_CONTROL_X;
      cfg81 -> interval = 50;
      cfg81 -> critical = 100;
      cfg81 -> sensorName = "joy_x";
      cfg81 -> enabled = true;
      Configurator::Instance().addConfig(cfg81);

      Joystick_control_config * cfg91 = new Joystick_control_config(id++);
      cfg91 -> sensorNameX = "joy_x";
      cfg91 -> neutralX = 512;
      cfg91 -> deadbandX = 50;
      cfg91 -> limitXMin = 0;
      cfg91 -> limitXMax = 1023;
      cfg91 -> invertX = true;
      cfg91 -> sensorNameY = "joy_y";
      cfg91 -> neutralY = 512;
      cfg91 -> deadbandY = 50;
      cfg91 -> limitYMin = 0;
      cfg91 -> limitYMax = 1023;
      cfg91 -> invertY = false;
      cfg91 -> sensorEnabled = "joy_enable";
      cfg91 -> enabled = true;
      Configurator::Instance().addConfig(cfg91);

/*
      //9
      //Force_control_config * cfg9 = new Force_control_config(id++);
      //cfg9 -> sensorName = "joy_y";
      //cfg9 -> enabled = false;
      //Configurator::Instance().addConfig(cfg9);

      */

      //10
      Exponential_power_modulation_config * cfg_modulation = new Exponential_power_modulation_config(id++);
      cfg_modulation -> enabled = true;
      Configurator::Instance().addConfig(cfg_modulation);

      //11
      Power_limit_config * cfg_limit = new Power_limit_config(id++);
      cfg_limit -> enabled = true;
      cfg_limit -> deadSensorName = "dead";
      Configurator::Instance().addConfig(cfg11);

/*
      //12
      Serial_control_config * cfg12 = new Serial_control_config(id++);
      cfg12 -> enabled = false;
      Configurator::Instance().addConfig(cfg12);
*/

      //16
      Pwm_steering_config * cfg16 = new Pwm_steering_config(id++);
//Perfect for most steering // ROBOT MODE
//      cfg16 -> minAngle = 30;
//      cfg16 -> maxAngle = 180;
//Perfect for driving  // FUN MODE
      cfg16 -> minAngle = 50;
      cfg16 -> maxAngle = 160;
      cfg16 -> neutral = 110;
      cfg16 -> enabled = true;
      Configurator::Instance().addConfig(cfg16);

/*
      //17
      Pwm_controller_config * cfg17 = new Pwm_controller_config(id++);
      cfg17 -> enabled = true;
      Configurator::Instance().addConfig(cfg17);
*/


      //18
      Web_update_config * cfg_web_update = new Web_update_config(id++);
      cfg_web_update -> enabled = true;
      Configurator::Instance().addConfig(cfg_web_update);

      //19
      Websocket_connection_config * cfg_websocket_connection = new Websocket_connection_config(id++);
      cfg_websocket_connection -> enabled = true;
      Configurator::Instance().addConfig(cfg_websocket_connection);

      //20
      //Websocket control gives a reset when combined with the new led driver
      Websocket_control_config * cfg19 = new Websocket_control_config(id++);
      cfg19 -> enabled = true;
      Configurator::Instance().addConfig(cfg19);

/*
      Websocket_drivelog_config * cfg26 = new Websocket_drivelog_config(id++);
      cfg26 -> enabled = false;
      Configurator::Instance().addConfig(cfg26);
*/

/*
      //22
      Websocket_server_log_config * cfg21 = new Websocket_server_log_config(id++);
      cfg21 -> enabled = true;
      cfg21 -> logLevel = LogLevel::INFO;
      Configurator::Instance().addConfig(cfg21);
*/
      /*
      //23
      Ntp_timeservice_config * cfg22 = new Ntp_timeservice_config(id++);
      cfg22 -> enabled = false;
      Configurator::Instance().addConfig(cfg22);

      //23
      Arduino_ota_config* cfg24 = new Arduino_ota_config(id++);
      Configurator::Instance().addConfig(cfg24);

      Esp32_digital_led_config * cfg27 = new Esp32_digital_led_config(id++);
      cfg27 -> enabled = false;
      Configurator::Instance().addConfig(cfg27);

      Mqtt_connection_config * cfg28 = new Mqtt_connection_config(id++);
      cfg28 -> enabled = false;
      Configurator::Instance().addConfig(cfg28);

      Mqtt_control_config * cfg29 = new Mqtt_control_config(id++);
      cfg29 -> enabled = false;
      Configurator::Instance().addConfig(cfg29);
*/
      //21
      Control_priority_config * cfg20 = new Control_priority_config(id++);
      cfg20 -> priority1 = cfg91 -> getId(); //Joystick
//      cfg20 -> priority1 = cfg14 -> getId(); //Wifi simple
      cfg20 -> priority2 = cfg19 -> getId(); //Websocket
  //    cfg20 -> priority3 = cfg29 -> getId(); //MQTT
    //  cfg20 -> priority4 = cfg12 -> getId(); //Serial
    //  cfg20 -> priority5 = cfg91 -> getId(); //Joystick
      Configurator::Instance().addConfig(cfg20);
    }
};

#endif
