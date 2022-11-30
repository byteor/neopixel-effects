#include "Fire.h"


void Fire::init(void)
{
  Serial.println("Fire2022: Hot Coal");
  Serial.printf("%d flames [%d, %d] width; Heat from %d to %d; Animation: %d\n", 
    NUM_FLAMES, MIN_FLAME_WIDTH, MAX_FLAME_WIDTH, MIN_HEAT_VALUE, MAX_HEAT_VALUE, ANIMATION_LENGTH);

  float step = 2 * PI / ANIMATION_LENGTH;
  for (int i = 0; i < ANIMATION_LENGTH; i++)
  {
    animation[i] = sin(step * i);
  }
  for (int i = 0; i < NUM_FLAMES; i++)
  {
    flames[i].progress = 0;
  }
  Serial.println("Hot Coal started");
}

void Fire::newFlame(void)
{
  byte width = random8(MIN_FLAME_WIDTH, MAX_FLAME_WIDTH);
  for (int i = 0; i < NUM_FLAMES; i++)
  {
    if (flames[i].progress == 0)
    {
      //Serial.print("*");
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

void Fire::setHeatValue(int y, int value)
{
  if (y >= 0 && y < NUM_LEDS)
  {
    heat[y] = DEFAULT_HEAT_VALUE + value;
  }
}

void Fire::processFlames(void)
{
  for (int i = 0; i < NUM_FLAMES; i++)
  {
    if (flames[i].progress > 0)
    {
      int newHeat = (float)flames[i].heat * animation[flames[i].progress];
      //Serial.printf("%d - %d * %f -> %d + %d\r\n", flames[i].y, flames[i].heat, animation[flames[i].progress], heat[flames[i].y], newHeat);
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

void Fire::keepAlive(void) 
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
    leds[j] = color;
  }
}

