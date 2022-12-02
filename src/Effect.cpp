#include "Effect.h"

void Effect::setup(void)
{
  // this resets all the neopixels to an off state
  Serial.printf("LEDs: %d, Frames: %d Per %dms\n", numLeds, framesPerTimeUnit, baseTimeUnitMs);
  if (strip == NULL || strip->PixelCount() != numLeds)
  {
    strip = new NeoPixel_t(numLeds, LED_PIN);
    init();
    strip->Begin();
    strip->Show();
  }
  else
    init();
}

void Effect::loop(void)
{
  if (millis() > timer + BASE_TIME_UNIT_MS / FRAMES_PER_UNIT)
  {
    timer = millis();

    keepAlive();

    RgbColor pixel;
    //Serial.printf("=== %d\n", brightness);
    for (int i = 0; i < numLeds; i++)
    {
      pixel = translateColor(leds[i]);//.Dim(brightness);
      // TODO: review the next line
      //pixel = RgbColor::LinearBlend(pixel, RgbColor(0, 0, 0), (255 - pixel.CalculateBrightness()) / 255.0);
      strip->SetPixelColor(i, pixel);
    }
    strip->Show();
  }
}

RgbColor Effect::translateColor(RgbColor color)
{
  uint8_t source[] = {color.R, color.G, color.B};
  uint8_t target[3];
  for (int i = 0; i < 3; i++)
  {
    target[(colorOrder >> 3 * (2 - i)) & 3] = source[i];
  }
  return RgbColor(target[0], target[1], target[2]);
}

RgbColor Effect::CRGB2RgbColor(CRGB color)
{
  return RgbColor(color.r, color.g, color.b);
}