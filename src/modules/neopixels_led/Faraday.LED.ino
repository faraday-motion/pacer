#if defined(ENABLELED)

#define LEDINTENSITYFRONT 100
#define LEDINTENSITYMAX 255
#define LEDINTENSITYHIGH 200
#define LEDINTENSITYLOW 100
#define LEDINTENSITYMIN 25

byte ledReadyColorCounter = 0;
byte ledReadyColor = 1;
byte ledReadyColorStep = 3;
const byte ledReadyColorMin = 0;
const byte ledReadyColorMax = 90;

byte blinkLeftIndex = 0;
byte blinkCount = 0;



void setupLeds()
{
  pinMode(PINPIXEL, OUTPUT);
  leds.begin();
}

void manageLeds()
{
  if (PROP_ERROR != ERRORS_NONE)
  {
    showLedErrorControls();
  }
  else if (isControllerEnabled() == false)
  {
    if (isDead() == false)
      ledReadyState();
    else
      ledDeadState();
  }
  else
  {
    showLedControls();
  }
  showFrontLights();
  showBackLights();
  updateLeds();
}

void updateLeds()
{
  if (leds.canShow())
    leds.show();
}

void showFrontLights()
{
  if (LEDSFRONTCOUNT > 0)
  {
    for (int i = LEDSFRONTINDEX; i < LEDSFRONTINDEX + LEDSFRONTCOUNT; i++) {
      if (PROP_LIGHTS == OFFON_ON)
        setPixelColor(i, 100, 100, 100);
      else
        setPixelColor(i, 0, 0, 0);
    }
  }
}

void showBackLights()
{
  int blinkLEDS = LEDSBACKCOUNT / 4;
  if (LEDSBACKCOUNT > 0)
  {
    for (int i = 0; i < LEDSBACKCOUNT; i++) {
      if (PROP_LIGHTS == OFFON_ON)
      {
        //If braking, put brake lights
        if (PROP_DRIVE == DRIVE_BRAKE)
          setPixelColor(LEDSBACKINDEX + i, LEDINTENSITYMAX, 0, 0);
        else
          setPixelColor(LEDSBACKINDEX + i, LEDINTENSITYLOW, 0, 0);

        if (PROP_BLINK == BLINK_RIGHT)
        {
          if (i < blinkLEDS)
          {
            if (i == blinkLeftIndex  || i == blinkLeftIndex + 1)
              setPixelColor(LEDSBACKINDEX + i, LEDINTENSITYMAX, LEDINTENSITYMAX, 0);
            else
              setPixelColor(LEDSBACKINDEX + i, LEDINTENSITYMIN, LEDINTENSITYMIN, 0);
          }
        }
        else if (PROP_BLINK == BLINK_LEFT)
        {
          if (i < blinkLEDS)
          {
            if (i == blinkLeftIndex  || i == blinkLeftIndex + 1)
              setPixelColor(LEDSBACKINDEX + LEDSBACKCOUNT - i, LEDINTENSITYMAX, LEDINTENSITYMAX, 0);
            else
              setPixelColor(LEDSBACKINDEX + LEDSBACKCOUNT - i, LEDINTENSITYMIN, LEDINTENSITYMIN, 0);
          }
        }
      }
      else
        setPixelColor(LEDSBACKINDEX + i, 0, 0, 0);
    }
  }
  blinkCount ++;
  if (blinkCount > 2)
  {
    blinkLeftIndex++;
    if (blinkLeftIndex > blinkLEDS)
      blinkLeftIndex = 0;
    blinkCount = 0;
  }
}

void showLedControls()
{
  if (LEDSCONTROLCOUNT > 0)
  {
    int pixelCountActual = map(motorPercent, 0, 100, 0, LEDSCONTROLCOUNT);
    int pixelCountTarget = map(motorTargetPercent, 0, 100, 0, LEDSCONTROLCOUNT);

    byte ledIndicatorSize = (byte)LEDSCONTROLCOUNT / 4;
    int ledNeutralStart = LEDSCONTROLINDEX + ((LEDSCONTROLCOUNT - ledIndicatorSize) / 2);
    int ledNeutralEnd = LEDSCONTROLINDEX + ((LEDSCONTROLCOUNT - ledIndicatorSize) / 2) + ledIndicatorSize;

    //If neutral
    if (PROP_DRIVE == DRIVE_NEUTRAL) {
      for (int i = LEDSCONTROLINDEX; i < LEDSCONTROLINDEX + LEDSCONTROLCOUNT; i++)
      {
        if (i >= ledNeutralStart && i < ledNeutralEnd )
          setPixelColor(i, LEDINTENSITYLOW, LEDINTENSITYLOW, 0);
        else
          setPixelColor(i, LEDINTENSITYMIN, LEDINTENSITYMIN, 0);
      }
    }
    else if (PROP_DRIVE == DRIVE_FORWARD)
    {
      //Going forward
      for (int i = 0; i < LEDSCONTROLCOUNT; i++) {
        //Always light up the first led
        if (i == 0)
          setPixelColor(LEDSCONTROLINDEX + i, 0, LEDINTENSITYHIGH, 0);
        else if (i < pixelCountActual)
          setPixelColor(LEDSCONTROLINDEX + i, 0, LEDINTENSITYHIGH, 0);
        else if (i < pixelCountTarget)
          setPixelColor(LEDSCONTROLINDEX + i, 0, LEDINTENSITYLOW, 0);
        else
          setPixelColor(LEDSCONTROLINDEX + i, 0, 0, 0);
      }
    }
    else if (PROP_DRIVE == DRIVE_BRAKE) {
      //Braking
      for (int i = 0; i < LEDSCONTROLCOUNT; i++) {
        //Always light up the first led
        if (i == 0)
          setPixelColor(LEDSCONTROLCOUNT + LEDSCONTROLINDEX - 1 - i, LEDINTENSITYHIGH, 0, 0);
        else if (i < pixelCountTarget)
          setPixelColor(LEDSCONTROLCOUNT + LEDSCONTROLINDEX - 1 - i, LEDINTENSITYLOW, 0, 0);
        else if (i < pixelCountActual)
          setPixelColor(LEDSCONTROLCOUNT + LEDSCONTROLINDEX - 1 - i, LEDINTENSITYHIGH, 0, 0);
        else
          setPixelColor(LEDSCONTROLCOUNT + LEDSCONTROLINDEX - 1 - i, 0, 0, 0);
      }
    }
  }
}

void showLedErrorControls()
{
  if (LEDSCONTROLCOUNT > 0)
  {
    for (int i = LEDSCONTROLINDEX; i < LEDSCONTROLINDEX + LEDSCONTROLCOUNT; i++)
    {
      if (i < LEDSCONTROLINDEX + PROP_ERROR)
        setPixelColor(i, LEDINTENSITYMIN, LEDINTENSITYHIGH, LEDINTENSITYLOW);
      else
        setPixelColor(i, 0, 0, 0);
    }
  }
}

void ledReadyState()
{
  if (LEDSCONTROLCOUNT > 0)
  {
    for (int i = LEDSCONTROLINDEX; i < LEDSCONTROLINDEX + LEDSCONTROLCOUNT; i++) {
      leds.setPixelColor(i, Wheel(ledReadyColorCounter));
    }
    if (ledReadyColorCounter <= ledReadyColorMin)
      ledReadyColor = +ledReadyColorStep;
    else if (ledReadyColorCounter >= ledReadyColorMax)
      ledReadyColor = -ledReadyColorStep;
    ledReadyColorCounter += ledReadyColor;
  }
}

void ledDeadState()
{
  if (LEDSCONTROLCOUNT > 0)
  {
    uint8_t i;
    static uint8_t c = 0;
    static uint8_t d = 5;
    for (i = LEDSCONTROLINDEX; i < LEDSCONTROLINDEX + LEDSCONTROLCOUNT; i++)
    {
      setPixelColor(i, c, 0, 0);
    }
    c += d;
    if ((c >= 255) || (c <= 0))
      d = -d;
  }
}

void setPixelColor(byte index, byte r, byte g, byte b)
{
  if (index >= 0 && index < LEDSTOTAL)
  {
    leds.setPixelColor(index, r, g, b);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return leds.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return leds.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return leds.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
#endif



