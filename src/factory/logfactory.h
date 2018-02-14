#ifndef LOGFACTORY_H
#define LOGFACTORY_H
#include <Arduino.h>
#include "../configuration/default/configuration.h"
#include "../configuration/base/configbase.h"
#include "../configuration/base/default_configbase.h"
#include "../enums/configurations.h"
#include "../fmv.h"

#include "../logs/base/logbase.h"
#include "../logs/spiffs_log/spiffs_log.h"
#include "../logs/serial_log/serial_log.h"
#include "../logs/websocket_server_log/websocket_server_log.h"
#include "../logs/logger.h"

class Logfactory{
public:

  static Logbase* getLogInstance(byte id, Configurations configuration, FMV * fmv)
  {
      Logbase* log = nullptr;
      switch(configuration) {
      case Configurations::SERIAL_LOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getLogInstance SERIAL_LOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        log = new Serial_log(id, fmv);
        break;
      case Configurations::SPIFFS_LOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getLogInstance SPIFFS_LOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        log = new Spiffs_log(id, fmv);
        break;
      case Configurations::WEBSOCKET_SERVER_LOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getLogInstance WEBSOCKET_SERVER_LOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        log = new Websocket_server_log(id, fmv);
        break;
      }
      return log;
  }
};

#endif
