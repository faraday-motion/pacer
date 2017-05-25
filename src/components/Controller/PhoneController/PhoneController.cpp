//#include <Arduino.h>
#include <stdint.h> // Required to be able to use uint8_t.
#include "PhoneController.h"
#include "../AbstractController.h"

// Construct the PhoneController and the AbstractController
PhoneController::PhoneController(ConfigController* configController, Wifi* wifi, byte controllerType, byte controllerId[])
 : AbstractController(configController, controllerType, controllerId)
{
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
       Serial.println();
       yield();

       if (PhoneController::validateChecksum(m, packetCount))
       {
           yield();
           processInput(m[4]);
       }
       else
       {
         //Checksum invalid
       }
     }
   } // endif serverClients are connected

} // end readFromWifiClient


// Placeholder for now.
void PhoneController::write()
{
  return;
}

// this is the loop
void PhoneController::handleController()
{
  Serial.println("handleController() (Phone)");
  read();
}

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
