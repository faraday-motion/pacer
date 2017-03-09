#if defined(ENABLENUNCHUK)

//Contains the previous checksum
byte previousNunchukChecksum = 0;
//How many times in a row was the checksum the same
int equalNunchukChecksumCount = 0;
//The max number of same checksums
int maxNunchukChecksumCount = 100;

//Nunchuk configuration
byte defaultNunchukMinBrake = 117;
byte defaultNunchukMaxBrake = 10;
byte defaultNunchukMinAcceleration = 137;
byte defaultNunchukMaxAcceleration = 235;
byte defaultNunchukNeutral = 127;

void readFromNunchukClient()
{
  nunchuk.update();
  yield();
#if defined(ENABLEDEVMODE)
  Serial.println("DATA");
  Serial.print(nunchuk.analogX, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.analogY, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.accelX, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.accelY, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.accelZ, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.zButton, DEC);
  Serial.print(' ');
  Serial.println(nunchuk.cButton, DEC);
#endif

  if (isNunchukDataValid(nunchuk))
  {
    yield();
    if (isNunchukChecksumValid(nunchuk))
    {
      yield();
      if (controlType == 0 || controlType == 2)
      {
        int nunchukY = constrain((byte)nunchuk.analogY, defaultNunchukMaxBrake, defaultNunchukMaxAcceleration);
#if defined(ENABLEDEVMODE)
        Serial.print("nunchukY: ");
        Serial.println(nunchukY);
#endif
        if (nunchukY > defaultNunchukMinBrake && nunchukY < defaultNunchukMinAcceleration )
        {
          //Neutral
#if defined(ENABLEDEVMODE)
          Serial.print("setSlaveInputPower FROM NUNCHUK NEUTRAL: ");
          Serial.println(defaultInputNeutral);
#endif
          controlEnabled = true;
          setPower(defaultInputNeutral, 2);
        }
        else if (nunchukY >= defaultNunchukMinAcceleration)
        {
          byte input = map(nunchukY, defaultNunchukMinAcceleration, defaultNunchukMaxAcceleration, defaultInputMinAcceleration, defaultInputMaxAcceleration);
#if defined(ENABLEDEVMODE)
          Serial.print("setSlaveInputPower FROM NUNCHUK ACCEL: ");
          Serial.println(input);
#endif
          controlEnabled = true;
          setPower(input, 2);
        }
        else
        {
          byte input = map(nunchukY, defaultNunchukMinBrake, defaultNunchukMaxBrake, defaultInputMinBrake, defaultInputMaxBrake);
#if defined(ENABLEDEVMODE)
          Serial.print("setSlaveInputPower FROM NUNCHUK BRAKE: ");
          Serial.println(input);
#endif
          controlEnabled = true;
          setPower(input, 2);
        }
        delay(1);

#if defined(ENABLEDEVMODE)
        Serial.println("DATA VALID");
        Serial.print(nunchuk.analogX, DEC);
        Serial.print(' ');
        Serial.print(nunchuk.analogY, DEC);
        Serial.print(' ');
        Serial.print(nunchuk.accelX, DEC);
        Serial.print(' ');
        Serial.print(nunchuk.accelY, DEC);
        Serial.print(' ');
        Serial.print(nunchuk.accelZ, DEC);
        Serial.print(' ');
        Serial.print(nunchuk.zButton, DEC);
        Serial.print(' ');
        Serial.println(nunchuk.cButton, DEC);
#endif
      }
    }
    else
    {
      //We had too many of the same checksum values in a row
      setDefaultPower();
#if defined(ENABLEDEVMODE)
      Serial.print("EqualChecksumCount: ");
      Serial.println(equalNunchukChecksumCount);
#endif
    }
  }
  else
  {
    //The nunchuk is not connected and the reciever is giving us garbage
#if defined(ENABLEDEVMODE)
    Serial.println("Connect Nunchuk");
#endif
  }
}

//Checks if the values returned from the nunchuk seems valid
bool isNunchukDataValid(ArduinoNunchuk nunchuk)
{
  if (nunchuk.analogX < 0 || nunchuk.analogX > 255)
    return false;
  if (nunchuk.analogY < 0 || nunchuk.analogY > 255)
    return false;
  if (nunchuk.accelX < 0 || nunchuk.accelX > 1023)
    return false;
  if (nunchuk.accelY < 0 || nunchuk.accelY > 1023)
    return false;
  if (nunchuk.accelZ < 0 || nunchuk.accelZ > 1023)
    return false;
  if (nunchuk.zButton < 0 || nunchuk.zButton > 1)
    return false;
  if (nunchuk.cButton < 0 || nunchuk.cButton > 1)
    return false;
  return true;
}

//Creates a checksum and checks if it is good
bool isNunchukChecksumValid(ArduinoNunchuk nunchuk)
{
  int sum = 0;
  sum += nunchuk.analogX;
  sum += nunchuk.analogY;
  sum += nunchuk.accelX;
  sum += nunchuk.accelY;
  sum += nunchuk.accelZ;
  sum += nunchuk.zButton;
  sum += nunchuk.cButton;
  //After first initialisation, there is a hickup and this resets this hickup, 3579 is the max value from all properties
  if (sum == 3579)
  {
    Wire.endTransmission(false);
    delay(1);
    nunchuk.init();
    delay(1);
    return false;
  }
  //Create the checksum
  byte checkSum = sum % 256;
  if (previousNunchukChecksum == checkSum)
    equalNunchukChecksumCount ++;
  else
    equalNunchukChecksumCount = 0;
  previousNunchukChecksum = checkSum;
  if (equalNunchukChecksumCount > maxNunchukChecksumCount)
    return false;
  return true;
}
#endif
