#include "Fire.h"

// HOT COAL FIRE effect
// using ws2812 LEDs
// The initial idea and code structure is based on Fire2012
// https://github.com/krzychb/EspFire2012

#define FRAMES_PER_SECOND 30

#define NUM_LEDS 300 // 60
CRGB leds[NUM_LEDS];

// Some strips have Blue and Green messed up
#define SWAP_BLUE_AND_GREEN false

// make sure to set this to the correct pin, ignored for Esp8266
#define LED_PIN 3

// NOTE: These will ignore the LED_PIN and use GPI03 pin
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(NUM_LEDS, LED_PIN);

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more calm fire.
#define SPARKLING 40

// NUM_FLAMES: The number of "Flames". 
// A Flame is a group of LEDs that heats up and cools down together.
// A Flame has a center LED that takes all the heat value, 
// and other LEDs take heat in proportion of I/WIDTH, where I is LED's position from the center of the Flame
#define NUM_FLAMES 60
#define MIN_FLAME_WIDTH 4
#define MAX_FLAME_WIDTH 10

// The length of an "animation". Pre-calculated sin() values are used to calculate the current temperature
// The total length of animation can be calculated as ANIMATION_LENGTH/FRAMES_PER_SECOND
#define ANIMATION_LENGTH 128

// A heat value a LED can cool down to
#define MIN_HEAT_VALUE 1
// A heat value a LED can heat up to
#define MAX_HEAT_VALUE 255
// A heat value a LED has when not involved in any Flame
#define DEFAULT_HEAT_VALUE (MIN_HEAT_VALUE + (MAX_HEAT_VALUE - MIN_HEAT_VALUE) / 2)

Flame flames[NUM_FLAMES];
byte heat[NUM_LEDS];
float animation[ANIMATION_LENGTH];

void setupFire(void)
{
  float step = 2 * PI / ANIMATION_LENGTH;
  for (int i = 0; i < ANIMATION_LENGTH; i++)
  {
    animation[i] = sin(step * i);
  }
  for (int i = 0; i < NUM_FLAMES; i++)
  {
    flames[i].progress = 0;
  }

  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();

  Serial.println("Hot Coal started");
}

void newFlame(void)
{
  byte width = random8(MIN_FLAME_WIDTH, MAX_FLAME_WIDTH);
  for (int i = 0; i < NUM_FLAMES; i++)
  {
    if (flames[i].progress == 0)
    {
      // animation direction - from hot to cold or vice versa
      int sgn = random8(0, 255) > 127 ? 1 : -1;

      flames[i].heat = sgn * DEFAULT_HEAT_VALUE;
      flames[i].width = random8(MIN_FLAME_WIDTH, MAX_FLAME_WIDTH);
      flames[i].speed = random8(1, 3);
      // Flames have certain widths and new flame should not intersect with ones in progress
      for (int j = 0; j < 5; j++)
      {
        int y = random16(NUM_LEDS);
        bool intersect = false;
        int thisStart = y - flames[i].width;
        int thisEnd = y + flames[i].width;
        for (int k = 0; k < NUM_FLAMES; k++)
        {
          int otherStart = flames[k].y - flames[k].width;
          int otherEnd = flames[k].y + flames[k].width;
          if (flames[k].progress > 0 
            && ((thisStart >= otherStart && thisStart <= otherEnd) || (thisEnd >= otherStart && thisEnd <= otherEnd)))
          {
            intersect = true;
            break;
          }
        }
        if(!intersect)
        {
          flames[i].y = y;
          flames[i].progress = 1;
          return;
        }
      }
      // Serial.println(flames[i].heat);
      return;
    }
  }
}

void setHeatValue(int y, int value)
{
  if (y >= 0 && y < NUM_LEDS)
  {
    heat[y] = DEFAULT_HEAT_VALUE + value;
  }
}

void processFlames(void)
{
  for (int i = 0; i < NUM_FLAMES; i++)
  {
    if (flames[i].progress > 0)
    {
      int newHeat = (float)flames[i].heat * animation[flames[i].progress];
      // Serial.printf("%d -> %d + %d\r\n", flames[i].y, heat[flames[i].y], newHeat);
      for (int j = 0; j < flames[i].width; j++)
      {
        int newValue = newHeat - j * newHeat / flames[i].width;
        setHeatValue(flames[i].y + j, newValue);
        setHeatValue(flames[i].y - j, newValue);
      }
      flames[i].progress += flames[i].speed;
      if (flames[i].progress >= ANIMATION_LENGTH)
        flames[i].progress = 0;
    }
  }
}

void Fire2022(void)
{
  // Step 1. Set all LEDs to default heat value
  for (int i = 0; i < NUM_LEDS; i++)
  {
    heat[i] = DEFAULT_HEAT_VALUE;
  }
  // Step 2. Process flames
  processFlames();

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < SPARKLING)
  {
    newFlame();
  }
  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < NUM_LEDS; j++)
  {
    CRGB color = HeatColor(heat[j]);
    //color.setRGB(heat[j], heat[j], heat[j]);
    if (SWAP_BLUE_AND_GREEN)
    {
      color.setRGB(color.r, color.b, color.g);
    }
    leds[j] = color;
  }
}

long fireTimer;

void keepFireAlive(void)
{
  if (millis() > fireTimer + 1000 / FRAMES_PER_SECOND)
  {
    fireTimer = millis();
    Fire2022();
    RgbColor pixel;
    for (int i = 0; i < NUM_LEDS; i++)
    {
      pixel = RgbColor(leds[i].r, leds[i].g, leds[i].b);
      pixel = RgbColor::LinearBlend(pixel, RgbColor(0, 0, 0), (255 - heat[i]) / 255.0);
      strip.SetPixelColor(i, pixel);
    }
    strip.Show();
  }
}