#include <Arduino.h>
#include "Wifi.h"
#include <ESP8266WiFi.h>
#include <IPAddress.h>
#include "../ConfigController/ConfigController.h"

Wifi::Wifi()
{
}

void Wifi::setup(ConfigController* configController)
{
  Serial.println("Wifi Setup Started");
  config = configController->config;
  handleClientInterval = new Metro(250);

  //Wifi
  // Store address of the WiFiServeir
  this->server = new WiFiServer(config->wifi.port);
  const char *ssid = config->wifi.ssid;
  const char *password = config->wifi.password;

  Serial.print("Configuring access point...");
  IPAddress address(config->wifi.ip[0], config->wifi.ip[1], config->wifi.ip[2], config->wifi.ip[3]);
  IPAddress subnet (config->wifi.subnet[0], config->wifi.subnet[1], config->wifi.subnet[2], config->wifi.subnet[3]);
  //If we dont disable the wifi, then there will be some issues with conncting to the device sometimes
  WiFi.disconnect(true);
  byte channel = config->wifi.channel;
  float wifiOutputPower = 20.5; //Max power
  WiFi.setOutputPower(wifiOutputPower);
  WiFi.setPhyMode(WIFI_PHY_MODE_11B);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_AP);
  //C:\Users\spe\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.1.0\cores\esp8266\core_esp8266_phy.c
  //TRYING TO SET [114] = 3 in core_esp8266_phy.c 3 = init all rf
  WiFi.persistent(false);
  WiFi.softAPConfig(address, address, subnet);
  WiFi.softAP(ssid, password, channel);
  IPAddress myIP = WiFi.softAPIP();

  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server->begin();
  //Set delay = true retarts the esp in version 2.1.0, check in later versions if its fixed
  server->setNoDelay(true);

  //Serial.println(server->status());
}

void Wifi::handleClientConnections() {
  //Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
  yield();
  if (handleClientInterval->check() == 1)
  {
    if (server->hasClient())
    {
      client = server->available();
      if (client && client.connected())
      {
        client.setNoDelay(true);
      }
      else
      {
        client.stop();
      }
    }
  }
  yield();
}
