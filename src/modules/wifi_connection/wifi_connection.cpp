#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include "./wifi_connection.h"
#include "../../configuration/default/configuration.h"

using namespace std;
using namespace std::placeholders;

//static byte sWiFiEventId = 0;

void Wifi_connection::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    //sWiFiEventId = 0;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Wifi_connection::loop()
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
    /*
    if (sWiFiEventId==1)
    {
      if (!MDNS.begin(mCfg -> host.c_str())) {
        Logger::Instance().write(LogLevel::WARNING, FPSTR("Error setting up MDNS responder!"));
      }
      Logger::Instance().write(LogLevel::INFO, FPSTR("WIFI_STA_STARTED"));
      onEvent(Events::WIFI_STA_STARTED);
      sWiFiEventId = 0;
    }
    else if (sWiFiEventId==2)
    {
      Logger::Instance().write(LogLevel::INFO, FPSTR("WIFI_STA_ENDED"));
      onEvent(Events::WIFI_STA_ENDED);
      sWiFiEventId = 0;
    }
    */
  }
}



void Wifi_connection::setWifiOff()
{
  Logger::Instance().write(LogLevel::INFO, FPSTR("WifiOff"));
  WiFi.mode(WIFI_OFF);
  delay(50);
  onEvent(Events::WIFI_DISABLED);
}

void Wifi_connection::setWifiAp()
{
  Logger::Instance().write(LogLevel::INFO, FPSTR("WifiAp"));
  WiFi.mode(WIFI_AP);
  delay(50);
  IPAddress apIP(10, 10, 100, 254);
  WiFi.softAP(mCfg -> ap_ssid.c_str(), mCfg -> ap_password.c_str(), mCfg -> ap_channel);
  delay(50);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  delay(50);
  onEvent(Events::WIFI_AP_STARTED);
}

void Wifi_connection::setWifiStation()
{
  Logger::Instance().write(LogLevel::INFO, FPSTR("WifiStation"));
  //WiFi.disconnect(true);
  //delay(250);
  //WiFi.onEvent(onWiFiEvent);
  WiFi.mode(WIFI_STA);
  WiFi.begin(mCfg -> ssid.c_str(), mCfg -> password.c_str());

  byte retries = 10;
  while (WiFi.status() != WL_CONNECTED && retries > 0) {
    delay(500);
    retries --;
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Wifi_connection IP address: "), Tools::ipAddressToString(WiFi.localIP()));
    onEvent(Events::WIFI_STA_STARTED);
  }
  else
    Logger::Instance().write(LogLevel::INFO, FPSTR("NO Wifi_connection"));
}

/*
void Wifi_connection::onWiFiEvent(WiFiEvent_t event)
{
    //Hack as i could not make the bind function work
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Logger::Instance().write(LogLevel::INFO, FPSTR("Wifi_connection IP address: "), String(WiFi.localIP()));
        sWiFiEventId = 1;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Logger::Instance().write(LogLevel::INFO, FPSTR("Wifi_connection lost connection"));
        sWiFiEventId = 2;
        break;
    }
}
*/

void Wifi_connection::command(byte command)
{
  if (enabled())
  {
    Commands comm = static_cast<Commands>(command);
    if (comm == Commands::CONNECTION_WIFI_OFF)
    {
      setWifiOff();
    }
    else if (comm == Commands::CONNECTION_WIFI_AP)
    {
      setWifiAp();
    }
    else if (comm == Commands::CONNECTION_WIFI_STA)
    {
      setWifiStation();
    }
  }
}

void Wifi_connection::onDisable()
{
    Logger::Instance().write(LogLevel::INFO, FPSTR("Wifi_connection::onDisable::WifiOff"));
    WiFi.mode(WIFI_OFF);
}
