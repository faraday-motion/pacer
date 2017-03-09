#if defined(ENABLEWIFI)

void setupWIFI()
{
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

  server.begin();
  //Set delay = true retarts the esp in version 2.1.0, check in later versions if its fixed
  server.setNoDelay(true);
}

void hasClients()
{
  uint8_t i;
  //Check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i])
          serverClients[i].stop();
        serverClients[i] = server.available();
        serverClients[i].setNoDelay(true);

        Serial.print("New client: ");;
        Serial.println(i);
        continue;
      }
      //Lets allow for background work
      yield();
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
    yield();
  }
}

void readFromWifiClient()
{
  uint8_t i;
  //Check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      unsigned int timeout = 3;
      unsigned long timestamp = millis();

      while (serverClients[i].available() == 0 && ((millis() - timestamp) <= timeout))
        yield();

      unsigned int streamLength = serverClients[i].available();
      if (streamLength > 0)
      {
#if defined(ENABLEDEVMODE)
        Serial.println("available:");
        Serial.println(streamLength);
#endif
        //size_t len
        byte packetLength = 11;
        byte packetCount = 0;
        byte m[packetLength];

        unsigned int packages = int(float(streamLength / packetLength));
#if defined(ENABLEDEVMODE)
        Serial.println("packages:");
        Serial.println(packages);
#endif
        if (packages > 0) {
          //Read all the first packages and keep only the last one
#if defined(ENABLEDEVMODE)
          Serial.print("Skipped packets:");
          Serial.println((packages - 1) * packetLength);
#endif
          for (uint8_t ii = 0; ii < (packages - 1) * packetLength; ii++)
          {
            serverClients[i].read();
          }
          for (uint8_t ii = 0; ii < packetLength; ii++)
          {
            m[packetCount++] = serverClients[i].read();
          }
          yield();


#if defined(ENABLEDEVMODE)
          unsigned long inputduration = millis() - lastinputduration;
          if (inputduration < mininputduration)
            mininputduration = inputduration;

          if (inputduration > maxinputduration)
            maxinputduration = inputduration;
          lastinputduration = millis();

          Serial.println("Inputduration:");
          Serial.println(inputduration);
          Serial.println("mininputduraton:");
          Serial.println(mininputduration);
          Serial.println("maxinputduration:");
          Serial.println(maxinputduration);
#endif
          if (validateChecksum(m, packetCount))
          {
            yield();
            //Set the power and specify controller 1, the wifi reciever
            if (controlType == 0 || controlType == 1)
            {
              controlEnabled = true;
              setPower(m[4], 1);
            }
          }
          else
          {
            //Checksum invalid
          }
        }
      }
    }
  }
}

byte getChecksum(byte* array, byte arraySize)
{
  long validateSum = 0;
  for (byte i = 0; i < arraySize; i++) {
    validateSum += array[i];
  }
  validateSum -= array[arraySize - 1];
  return validateSum % 256;
}

bool validateChecksum(byte* array, byte arraySize)
{
  return array[arraySize - 1] == getChecksum(array, arraySize);
}

#endif
