#ifndef ConfigStructs_h
#define ConfigStructs_h
#include <Arduino.h> // for the byte type

// Structures wifi config data read from teh SPIFFS.
struct _wifiConfig {
  const char* ssid;
  const char* password;
  int port;
  int ip[4];
  int subnet[4];
  int channel;
};

// Structures controller config data read from teh SPIFFS.
struct _controllerConfig {
  byte defaultInputNeutral;
  byte defaultInputMinBrake;
  byte defaultInputMaxBrake;
  byte defaultInputMinAcceleration;
  byte defaultInputMaxAcceleration;
  float defaultSmoothAlpha;
};

// Structures currentControl config data read from teh SPIFFS.
struct _currentControlConfig {
  float defaultCurrentNeutral;
  float defaultCurrentAccelerationMax;
  float defaultCurrentAccelerationMin;
  float defaultCurrentBrakeMax;
  float defaultCurrentBrakeMin;
};


#endif
