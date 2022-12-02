#pragma once

#include "Effect.h"
// HOT COAL FIRE effect
// using ws2812 LEDs
// The initial idea and code structure is based on Fire2012
// https://github.com/krzychb/EspFire2012


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
// The total length of animation can be calculated as ANIMATION_LENGTH/FRAMES_PER_UNIT
#define ANIMATION_LENGTH 128

// A heat value a LED can cool down to
#define MIN_HEAT_VALUE 20
// A heat value a LED can heat up to
#define MAX_HEAT_VALUE 150
// A heat value a LED has when not involved in any Flame
#define DEFAULT_HEAT_VALUE (MIN_HEAT_VALUE + (MAX_HEAT_VALUE - MIN_HEAT_VALUE) / 2)

struct Flame
{
  int y;
  byte width;
  int heat;
  byte speed;
  byte progress;
};

class Fire : public Effect
{
protected:
  Flame flames[NUM_FLAMES];
  byte heat[MAX_NUM_LEDS];
  float animation[ANIMATION_LENGTH];

  void setHeatValue(int y, int value);
  void newFlame(void);
  void Fire2022(void);
  void processFlames(void);
  void init(void);
public:
  Fire(int numPixels, EOrder colorOrder) : Effect(numPixels, colorOrder){};
  void keepAlive(void);
};
