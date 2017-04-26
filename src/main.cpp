// Required includes
#include <Metro.h>
#include <Ticker.h> // used for the Vesc.
#include <Arduino.h>
#include "components/Motor/Motor.h"
#include "components/Connection/Wifi.h"
#include "components/Controller/Controller.h"
#include "components/Controller/PhoneController.h"
#include "components/MotorController/MotorController.h"

// temp
#include <ESP8266WiFi.h>
// temp

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
Metro metroControllerCommunication = Metro(500);
/***********************************************/


/***********************************************/
/** TODO:: Wrap up in a builder of some sort  **/

// Independent Objects
WiFiServer      wifiServer(8899); // TODO::Get the port from a config file.
Wifi            wifi;
MotorController motorController;

// Dependent Objects
Motor           motor(&motorController);
Controller      controller;
PhoneController phoneController;
/***********************************************/


void updateMotorController(int i)
{
  motorController.update();
}
// Setup the Motor Cotnroller.
void setupMotorController()
{
  motorControllerTicker.attach_ms(1, updateMotorController, 0);
}

void setup() {
  Serial.begin(115200);
  wifi.setup(&wifiServer);
  controller.setup(&motorController);
  phoneController.setup(&controller, &wifi);
  setupMotorController();
}


void loop() {

  // Check if clients want to connect to Wifi AP Server.
  while (metro250ms.check() == 1)          wifi.registerClient();
  while (metroControllerRead.check() == 1) phoneController.read();

  yield();
}
