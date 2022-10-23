#include "Effect.h"


void Effect::setup(void)
{
  // this resets all the neopixels to an off state
  strip->Begin();
  strip->Show();
}

void Effect::loop(void)
{
  if (millis() > timer + 1000 / FRAMES_PER_SECOND)
  {
    timer = millis();

    keepAlive();

    RgbColor pixel;
    for (int i = 0; i < NUM_LEDS; i++)
    {
      pixel = RgbColor(leds[i].r, leds[i].g, leds[i].b);
      if (SWAP_BLUE_AND_GREEN)
      {
        pixel = RgbColor(leds[i].r, leds[i].b, leds[i].g);
      }
      pixel = RgbColor::LinearBlend(pixel, RgbColor(0, 0, 0), (255 - pixel.CalculateBrightness()) / 255.0);
      strip->SetPixelColor(i, pixel);
    }
    strip->Show();
  }
}