//#include <Arduino.h>
#include <stdint.h> // Required to be able to use uint8_t.
#include "PhoneController.h"
#include "Controller.h"
#include "components/Motor/Motor.h"

PhoneController::PhoneController() {

}

void PhoneController::setup(Controller* controller, Wifi* wifi)
{
  this->controller = controller;
  this->wifi = wifi;
}

void PhoneController::read()
{
 uint8_t i;
 // Check clients for data
   if (wifi->client && wifi->client.connected()) {
     unsigned int  timeout   = 3;
     unsigned long timestamp = millis();

     // While no data is coming do yield();
     while (wifi->client.available() == 0 && ((millis() - timestamp) <= timeout)) {
       yield();
     }

     unsigned int streamLength = wifi->client.available();
     if (streamLength > 0) {
       // size_t len
       byte packetLength = 11;
       byte packetCount  = 0;
       byte m[packetLength];

       unsigned int packages = int(float(streamLength / packetLength));

       for (uint8_t ii = 0; ii < (packages - 1) * packetLength; ii++)
       {
         wifi->client.read();
       }

       for (uint8_t ii = 0; ii < packetLength; ii++)
       {
         m[packetCount++] = wifi->client.read();
       }
       yield();

       if (PhoneController::validateChecksum(m, packetCount))
       {
         yield();
         //Set the power and specify controller 1, the wifi reciever
         if (controller->controllerType == 0 || controller->controllerType == 1)
         {
           controller->controlEnabled = true;
           controller->smoothenInput(m[4]);
         }
       }
       else
       {
         //Checksum invalid
       }
     }
   } // endif serverClients are connected

} // end readFromWifiClient

byte PhoneController::getChecksum(byte* array, byte arraySize)
{
  long validateSum = 0;
  for (byte i = 0; i < arraySize; i++) {
    validateSum += array[i];
  }
  validateSum -= array[arraySize - 1];
  return validateSum % 256;
}

bool PhoneController::validateChecksum(byte* array, byte arraySize)
{
  return array[arraySize - 1] == getChecksum(array, arraySize);
}
