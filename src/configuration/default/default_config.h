#ifndef DEFAULT_CONFIG_H
#define DEFAULT_CONFIG_H

#include <Arduino.h>
#include "../base/default_configbase.h"
#include "./configuration.h"
#include "../configurator.h"
#include "../../enums/loglevel.h"
#include "../../modules/modules.hpp"
#include "../wheel_config.h"

class Default_config : public Default_configbase{
public:
  Default_config() : Default_configbase() {

  }

  virtual void addConfigs()
  {
      byte id = 0;

      //0
      Serial_log_config* cfg0 = new Serial_log_config(id++);
      cfg0 -> logLevel = LogLevel::DEBUG;
      cfg0 -> enabled = true;
      Configurator::Instance().addConfig(cfg0);

      //15
      Vesc_controller_config* cfg15 = new Vesc_controller_config(id++);
      cfg15 -> defaultSerial = false;
      cfg15 -> enabled = true;
      Configurator::Instance().addConfig(cfg15);

      //The drivelog needs refactoring to tasks as it performs bad ans stalls normal execution
      Spiffs_drivelog_config* cfg23 = new Spiffs_drivelog_config(id++);
      cfg23 -> enabled = false;
      Configurator::Instance().addConfig(cfg23);

      //1
      Wheel_config* cfg4 = new Wheel_config(id++);
      cfg4 -> index = 0;
      cfg4 -> isElectric = true;
      cfg4 -> canSteer = false;
      cfg4 -> isInverse = false;
      Configurator::Instance().addConfig(cfg4);

      //2
      Wheel_config* cfg5 = new Wheel_config(id++);
      cfg5 -> index = 1;
      cfg5 -> isElectric = false;
      cfg5 -> canSteer = false;
      cfg5 -> isInverse = false;
      Configurator::Instance().addConfig(cfg5);

      //3
      Wheel_config* cfg6 = new Wheel_config(id++);
      cfg6 -> index = 2;
      cfg6 -> isElectric = false;
      cfg6 -> canSteer = true;
      cfg6 -> isInverse = false;
      Configurator::Instance().addConfig(cfg6);

      //4
      Wheel_config* cfg7 = new Wheel_config(id++);
      cfg7 -> index = 3;
      cfg7 -> isElectric = false;
      cfg7 -> canSteer = true;
      cfg7 -> isInverse = false;
      Configurator::Instance().addConfig(cfg7);

      //5
      //If the wifi is configured late it gives crashes for some reason
      Wifi_connection_config* cfg13 = new Wifi_connection_config(id++);
      cfg13 -> enabled = true;
      Configurator::Instance().addConfig(cfg13);

      //6
      Wifi_simple_control_config* cfg14 = new Wifi_simple_control_config(id++);
      cfg14 -> enabled = true;
      Configurator::Instance().addConfig(cfg14);

      //7
      Boolean_sensor_config * cfg1 = new Boolean_sensor_config(id++);
      cfg1 -> pin = PIN_DEAD_MAN_SWITCH;
      cfg1 -> interval = 100;
      cfg1 -> critical = 200;
      cfg1 -> invert = false;
      cfg1 -> sensorName = "dead";
      Configurator::Instance().addConfig(cfg1);

/*
      //9
      Neopixels_config* cfg3 = new Neopixels_config(id++);
      Configurator::Instance().addConfig(cfg3);
*/
      //10
      Analog_sensor_config* cfg8 = new Analog_sensor_config(id++);
      cfg8 -> pin = PIN_JOYSTICK_CONTROL_Y;
      cfg8 -> interval = 25;
      cfg8 -> critical = 50;
      cfg8 -> sensorName = "joy_y";
      Configurator::Instance().addConfig(cfg8);

      //11
      Force_control_config* cfg9 = new Force_control_config(id++);
      cfg9 -> sensorName = "joy_y";
      cfg9 -> enabled = true;
      Configurator::Instance().addConfig(cfg9);

      //12
      Exponential_power_modulation_config* cfg10 = new Exponential_power_modulation_config(id++);
      cfg10 -> enabled = true;
      Configurator::Instance().addConfig(cfg10);

      //13
      Power_limit_config* cfg11 = new Power_limit_config(id++);
      cfg11 -> enabled = true;
      cfg11 -> deadSensorName = "dead";
      Configurator::Instance().addConfig(cfg11);

      //14
      Serial_control_config* cfg12 = new Serial_control_config(id++);
      cfg12 -> enabled = true;
      Configurator::Instance().addConfig(cfg12);

      //16
      Pwm_steering_config* cfg16 = new Pwm_steering_config(id++);
      cfg16 -> enabled = true;
      Configurator::Instance().addConfig(cfg16);

      //17
      Pwm_controller_config* cfg17 = new Pwm_controller_config(id++);
      cfg17 -> enabled = true;
      Configurator::Instance().addConfig(cfg17);
/*
      //18
      Web_update_config* cfg18 = new Web_update_config(id++);
      cfg18 -> enabled = true;
      Configurator::Instance().addConfig(cfg18);
*/

      //19
      Websocket_connection_config* cfg25 = new Websocket_connection_config(id++);
      cfg25 -> enabled = true;
      Configurator::Instance().addConfig(cfg25);

      //20
      //Websocket control gives a reset when combined with the new led driver
      Websocket_control_config* cfg19 = new Websocket_control_config(id++);
      cfg19 -> enabled = true;
      Configurator::Instance().addConfig(cfg19);

      Websocket_drivelog_config* cfg26 = new Websocket_drivelog_config(id++);
      cfg26 -> enabled = true;
      Configurator::Instance().addConfig(cfg26);

      //21
      Control_priority_config* cfg20 = new Control_priority_config(id++);
      cfg20 -> enabled = true;
      cfg20 -> priority1 = cfg14 -> getId(); //Wifi simple
      cfg20 -> priority2 = cfg19 -> getId(); //Websocket
      cfg20 -> priority3 = cfg12 -> getId(); //Serial
      cfg20 -> priority4 = cfg9 -> getId(); //Force
      Configurator::Instance().addConfig(cfg20);

      //22
      Websocket_server_log_config* cfg21 = new Websocket_server_log_config(id++);
      cfg21 -> enabled = false;
      cfg21 -> logLevel = LogLevel::INFO;
      Configurator::Instance().addConfig(cfg21);

      //23
      Ntp_timeservice_config* cfg22 = new Ntp_timeservice_config(id++);
      cfg22 -> enabled = true;
      Configurator::Instance().addConfig(cfg22);

/*
      //23
      Arduino_ota_config* cfg24 = new Arduino_ota_config(id++);
      Configurator::Instance().addConfig(cfg24);
*/

      Esp32_digital_led_config* cfg27 = new Esp32_digital_led_config(id++);
      cfg27 -> enabled = true;
      Configurator::Instance().addConfig(cfg27);

    }
};

#endif
