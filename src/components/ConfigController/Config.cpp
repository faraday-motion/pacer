#include "Config.h"

Config::Config()
{
}

void Config::setConfig(JsonObject& json)
{
  this->configureWiredDevices(json);
  delay(5);
  this->configureWifi(json);
  delay(5);
  this->configureVehicle(json);
  delay(5);
  this->configureWebSockets(json);
  delay(5);
  this->configureModules(json);
}

void Config::configureWiredDevices(JsonObject& json)
{
  // Counters
  this->wiredDevicesCount = json["wiredDevicesCount"];
  // // Loop through all default wired devices.
  for (JsonObject::iterator it=json.begin(); it!=json.end(); ++it)
  {
    if(!strcmp(it->key, "wiredDevices")){
      size_t length = it->value.size();
      for (size_t i = 0; i < length; i++) {
        const char* id = it->value[i]["id"];
        this->wiredDevices[i].id[0] = id[0];
        this->wiredDevices[i].id[1] = id[1];
        this->wiredDevices[i].id[2] = id[2];
        this->wiredDevices[i].id[3] = id[3];
        this->wiredDevices[i].id[4] = id[4];
        this->wiredDevices[i].type = it->value[i]["type"];
        this->wiredDevices[i].priority = it->value[i]["priority"];
        this->wiredDevices[i].enabled = it->value[i]["enabled"];
        // NOTE:: We always expect the keys for constraints.
        this->wiredDevices[i].accelConstraint = it->value[i]["constraints"]["accel"];
        this->wiredDevices[i].brakeConstraint = it->value[i]["constraints"]["brake"];
      }
    }
  }
}

void Config::configureWifi(JsonObject& json)
{
  // Wifi
  this->wifi.ssid      = json["wifi"]["ssid"].as<char*>();
  this->wifi.password  = json["wifi"]["pass"].as<char*>();
  this->wifi.port      = json["wifi"]["port"];
  this->wifi.channel   = json["wifi"]["channel"];
  this->wifi.ip[0]     = json["wifi"]["ip"][0];
  this->wifi.ip[1]     = json["wifi"]["ip"][1];
  this->wifi.ip[2]     = json["wifi"]["ip"][2];
  this->wifi.ip[3]     = json["wifi"]["ip"][3];
  this->wifi.subnet[0] = json["wifi"]["subnet"][0];
  this->wifi.subnet[1] = json["wifi"]["subnet"][1];
  this->wifi.subnet[2] = json["wifi"]["subnet"][2];
  this->wifi.subnet[3] = json["wifi"]["subnet"][3];
}



void Config::configureWebSockets(JsonObject& json)
{
  // Websocket
  this->websocket.port = json["websocket"]["port"];
}

void Config::configureVehicle(JsonObject& json)
{
  // Motor Count
  this->motorCount = json["motorCount"];
  // controller
  this->controller.defaultSmoothAlpha   = json["controller"]["defaultSmoothAlpha"];
  this->controller.defaultInputNeutral  = json["controller"]["defaultInputNeutral"];
  this->controller.defaultInputMinBrake = json["controller"]["defaultInputMinBrake"];
  this->controller.defaultInputMaxBrake = json["controller"]["defaultInputMaxBrake"];
  this->controller.defaultInputMinAcceleration = json["controller"]["defaultInputMinAcceleration"];
  this->controller.defaultInputMaxAcceleration = json["controller"]["defaultInputMaxAcceleration"];
  // currentControl
  this->currentControl.defaultCurrentNeutral  = json["currentControl"]["defaultCurrentNeutral"];
  this->currentControl.defaultCurrentBrakeMin = json["currentControl"]["defaultCurrentBrakeMin"];
  this->currentControl.defaultCurrentBrakeMax = json["currentControl"]["defaultCurrentBrakeMax"];
  this->currentControl.defaultCurrentAccelerationMin = json["currentControl"]["defaultCurrentAccelerationMin"];
  this->currentControl.defaultCurrentAccelerationMax = json["currentControl"]["defaultCurrentAccelerationMax"];
}

void Config::configureModules(JsonObject& json)
{
  this->modules.radio = json["modules"]["radio"];
  this->modules.webServer = json["modules"]["webServer"];
  this->modules.webSocketServer = json["modules"]["webSocketServer"];
}

// Debug

void Config::printConfig()
{
  this->printWifi();
  delay(10);
  this->printWiredDevices();
}


void Config::printWifi()
{
  Serial.println("::Wifi Configuration::");
  Serial.print("SSID: ");
  Serial.println(this->wifi.ssid);
  Serial.print("PORT: ");
  Serial.println(this->wifi.port);
  Serial.print("Channel: ");
  Serial.println(this->wifi.channel);
  Serial.print("Password: ");
  Serial.println(this->wifi.password);
  Serial.print("IP: ");
  Serial.println();
  Serial.print(this->wifi.ip[0]);
  Serial.print(" ");
  Serial.print(this->wifi.ip[1]);
  Serial.print(" ");
  Serial.print(this->wifi.ip[2]);
  Serial.print(" ");
  Serial.print(this->wifi.ip[3]);
  Serial.println(" ");
  Serial.print("Subnet: ");
  Serial.println();
  Serial.print(this->wifi.subnet[0]);
  Serial.print(" ");
  Serial.print(this->wifi.subnet[1]);
  Serial.print(" ");
  Serial.print(this->wifi.subnet[2]);
  Serial.print(" ");
  Serial.print(this->wifi.subnet[3]);
  Serial.println(" ");
}


void Config::printWiredDevices()
{
  Serial.println(":::WiredDevices Configuration:::");

  Serial.print("Configured Devices Count: ");
  Serial.println(this->wiredDevicesCount);
  for (size_t i = 0; i < this->wiredDevicesCount; i++)
  {
    Serial.print("Iteration = ");
    Serial.println(i);

    Serial.print("ID: ");
    Serial.print(this->wiredDevices[i].id[0]);
    Serial.print(" ");
    Serial.print(this->wiredDevices[i].id[1]);
    Serial.print(" ");
    Serial.print(this->wiredDevices[i].id[2]);
    Serial.print(" ");
    Serial.print(this->wiredDevices[i].id[3]);
    Serial.print(" ");
    Serial.print(this->wiredDevices[i].id[4]);
    Serial.println();
    Serial.print("Type: ");
    Serial.println(this->wiredDevices[i].type);
    Serial.print("Priority: ");
    Serial.println(this->wiredDevices[i].priority);
    Serial.print("Enabled: ");
    Serial.println(this->wiredDevices[i].enabled);
    Serial.print("Brake Constraints");
    Serial.println(this->wiredDevices[i].brakeConstraint);
    Serial.print("Acceleration Constraints");
    Serial.println(this->wiredDevices[i].accelConstraint);
  }
}
