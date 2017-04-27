// Required includes
#include <Metro.h>
#include <Ticker.h> // used for the Vesc.
#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include "components/Motor/Motor.h"
#include "components/Connection/Wifi.h"
#include "components/Controller/Controller.h"
#include "components/Controller/PhoneController.h"
#include "components/MotorController/MotorController.h"


//temp
#include "FS.h"
#include "components/ConfigController/ConfigController.h"
//temp
/**************************************************/
/** TODO:: port these to the .ini file           **/
#define ENABLEDEVMODE //Output debugging information
#define ENABLEVESC //Enable the VESC
#define ENABLESMOOTHING //Enable smoothing.
/**************************************************/

/******************************************************/
/** TODO:: Have a class for pin configuration (.ini) **/
//#define PINEXTERNALRESET 16 // used in the brain only.
#define PINDEADSWITCH 12
// #define PINSERVOESC 0     //not used
// #define PINSERVOBRAKE1 2  //not used
// #define PINSERVOBRAKE2 14 //not used
// #define PINPIXEL 3        //not use
/******************************************************/
/**********************************************************/
/** TODO:: MotorController Setup that needs to be ported **/

Ticker motorControllerTicker; //TODO:: find a way to pass a callback that is a object memeber function
                              //TODO:: port this to the motorcontroller object


/** MotorController Setup that needs to be ported **/
/*********************************************************/



/******************************************************/
/** TODO:: Metro Timers need to be ported to class  **/
// Metro Timers

Metro metro250ms                   = Metro(250);
Metro metroControllerRead          = Metro(50) ;
Metro metroControllerCommunication = Metro(500); // TODO:: implement timer to check if the connection is lost.
/***********************************************/


/***********************************************/
/** TODO:: Wrap up in a builder of some sort  **/

// Independent Objects

Wifi            wifi;
WiFiServer      wifiServer(8899); // TODO::Get the port from a config file.
MotorController motorController;


// Dependent Objects
Controller      controller;
PhoneController phoneController;
/***********************************************/

ConfigController configController;


void updateMotorController(int i)
{
  motorController.update();
}

void vescSend(unsigned char *data, unsigned int len)
{

  Serial.println("Package Sent to VESC: ");
  Serial.print(data[0]);
  Serial.print(" | ");
  Serial.print(data[1]);
  Serial.print(" | ");
  Serial.print(data[2]);
  Serial.print(" | ");
  Serial.print(data[3]);
  Serial.print(" | ");
  Serial.print(data[4]);
  Serial.print(" | ");
  Serial.print(data[5]);
  Serial.print(" | ");
  Serial.print(data[6]);
  Serial.print(" | ");
  Serial.print(data[7]);
  Serial.print(" | ");
  Serial.print(data[8]);
  Serial.print(" | ");
  Serial.print(data[9]);
  Serial.print(" | ");
  Serial.print(data[10]);
  Serial.print(" | ");
  Serial.print(data[11]);
  Serial.print(" | ");
  Serial.print(data[12]);
  Serial.println(" | ");
  Serial.write(data, len);
}

void vescProcess(unsigned char *data, unsigned int len)
{
  // Serial.println("DATA COMING FROM VESC");
  // Serial.println(*data);
}

// Setup the Motor Cotnroller.
void setupMotorController()
{
  motorControllerTicker.attach_ms(1, updateMotorController, 0);
  motorController.uartInit(vescSend);
}

void setup() {
  Serial.begin(115200);
  wifi.setup(&wifiServer);
  controller.setup(&motorController);
  phoneController.setup(&controller, &wifi);
  setupMotorController();

  Serial.println("Mounting File System");
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount File System");
    return;
  }

  delay(2000);
  if(!configController.getConfig()) {
    Serial.println("Failed to load config");
  } else {
    Serial.println("Config Loaded");
  }

  // delay(5000);
  // if(!configController.saveConfig()) {
  //   Serial.println("Failed to save config");
  // } else {
  //   Serial.println("Config saved");
  // }
}


void loop() {

  // Check if clients want to connect to Wifi AP Server.
  while (metro250ms.check() == 1)          wifi.registerClient();
  while (metroControllerRead.check() == 1) phoneController.read();
  yield();
}
