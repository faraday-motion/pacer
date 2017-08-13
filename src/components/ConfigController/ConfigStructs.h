#ifndef ConfigStructs_h
#define ConfigStructs_h
#include <Arduino.h> // for the byte type

// Structures wifi config data read from teh SPIFFS.
struct _wifiConfig {
  String ssid;
  String password;
  int port;
  int ip[4];
  int subnet[4];
  int channel;
};

struct _webscocket {
  int port;
};

// Structures controller config data read from the SPIFFS.
struct _controllerConfig {
  byte defaultInputNeutral;
  byte defaultInputMinBrake;
  byte defaultInputMaxBrake;
  byte defaultInputMinAcceleration;
  byte defaultInputMaxAcceleration;
  float defaultSmoothAlpha;
};

// Structures currentControl config data read from the SPIFFS.
struct _currentControlConfig {
  float defaultCurrentNeutral;
  float defaultCurrentAccelerationMax;
  float defaultCurrentAccelerationMin;
  float defaultCurrentBrakeMax;
  float defaultCurrentBrakeMin;
};

// Structures wireDevices config data read from the SPIFFS.
struct _wiredDevice {
  byte id[5];
  byte type;
  byte priority;
  bool enabled;
  int  accelConstraint = 0;
  int  brakeConstraint = 0;
};

#endif
