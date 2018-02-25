#ifndef NTP_TIMESERVICE_CONFIG_H
#define NTP_TIMESERVICE_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Ntp_timeservice_config : public Configbase{
public:
  Ntp_timeservice_config(byte id) : Configbase(id, Configurations::NTP_TIMESERVICE_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["timeServer"] = timeServer;
    root["timeZone"] = timeZone;
    root["dateTimeFormat"] = dateTimeFormat;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    timeServer = root["timeServer"].as<String>();
    timeZone = root["timeZone"].as<String>();
    dateTimeFormat = root["dateTimeFormat"].as<String>();
  }

  String timeServer = NTP_TIME_SERVER;
  String timeZone = NTP_TIME_TIMEZONE;
  String dateTimeFormat = NTP_DATETIME_FORMAT;
};

#endif
