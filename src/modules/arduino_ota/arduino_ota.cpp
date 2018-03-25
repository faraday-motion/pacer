#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include "./arduino_ota.h"
#include "../../configuration/default/configuration.h"

void Arduino_ota::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Arduino_ota"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    // Port defaults to 3232
    if (mCfg -> port != 0)
      ArduinoOTA.setPort(mCfg -> port);

    // Hostname defaults to esp3232-[MAC]
    if (mCfg -> hostName != "")
      ArduinoOTA.setHostname(mCfg -> hostName.c_str());

    if (mCfg -> password != "")
      ArduinoOTA.setPassword(mCfg -> password.c_str());

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA
    .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        //Serial.println("Start updating " + type);
        Logger::Instance().write(LogLevel::INFO, FPSTR("Start updating "), String(type));
      })
      .onEnd([]() {
        //Serial.println("\nEnd");
        Logger::Instance().write(LogLevel::INFO, FPSTR("End"));
      })
      .onProgress([](unsigned int progress, unsigned int total) {
//        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Progress: "), String((progress / (total / 100))));
      })
      .onError([](ota_error_t error) {
        //Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Wifi_connection"));
         Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
      });

    ArduinoOTA.begin();
    Logger::Instance().write(LogLevel::INFO, FPSTR("ArduinoOTA IP address: "), Tools::ipAddressToString(WiFi.localIP()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Arduino_ota"));
    mIsSetup = true;
  }
}

void Arduino_ota::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
      ArduinoOTA.handle();
  }
}

void Arduino_ota::command(byte command)
{
  if (enabled())
  {
    Commands comm = static_cast<Commands>(command);
    /*
    if (comm == Commands::CONNECTION_WIFI_OFF)
    {
    }
*/
  }
}

void Arduino_ota::onDisable()
{

}
