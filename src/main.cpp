#include <Metro.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "components/Connection/Wifi.h"
#include "components/Controller/AbstractController.h"
#include "components/Controller/PhoneController.h"
#include "components/Controller/ControllerManager.h"
#include "components/MotorController/MotorController.h"
#include "components/ConfigController/ConfigController.h"


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
Config config;
Wifi              wifi;
WiFiServer        wifiServer(8899); // TODO::Get the port from a config file. Not sure how to do it as it need to be in this global scope where we can't execute ConfigController::getConfig(&Config);
MotorController   motorController;
ConfigController  configController(&config); // TODO:: see if we can instantiate the Config.h directly in the ConfigController.
ControllerManager controllerManager(&configController, &motorController, &wifi);
/***********************************************/

void setup() {
  Serial.begin(115200);
  configController.loadConfig();
  wifi.setup(&wifiServer, &configController);
  motorController.setup();

  controllerManager.registerController(1, 3); // type = 1, id = 3
  controllerManager.setActiveController(3);
}


void loop() {

  // Check if clients want to connect to Wifi AP Server.
  while (metro250ms.check() == 1)          wifi.registerClient();
  yield();
  while (metroControllerRead.check() == 1) controllerManager.listenToController();
  yield();
  //while (Serial.available() > 0) motorController.processUartByte(Serial.read());
}
