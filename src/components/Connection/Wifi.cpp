//#include <Arduino.h>
#include "Wifi.h"
#include <ESP8266WiFi.h>
#include <IPAddress.h>

//How many clients should be able to connect to this ESP8266
//#define MAX_SRV_CLIENTS 5


Wifi::Wifi()
{
}

void Wifi::setup(WiFiServer* wifiServer)
{
  // Store address of the wifiServer
  server = wifiServer;
  Serial.println("Wifi Setup Started");
  //Wifi
  const char *ssid = "FMV-OOP";
  const char *password = "faraday100";
  const int port = 8899;

  MAX_SRV_CLIENTS = 5;
  Serial.print("Configuring access point...");
  IPAddress address(10, 10, 100, 254);
  IPAddress subnet(255, 255, 255, 0);
  //If we dont disable the wifi, then there will be some issues with conncting to the device sometimes
  WiFi.disconnect(true);
  byte channel = 11;
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

void Wifi::registerClient() {
  //Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
  yield();
  if (server->hasClient()){
    client = server->available();
    if (client && client.connected()) {
      client.setNoDelay(true);
    } else {
      client.stop();
    }
  }
  yield();
}
