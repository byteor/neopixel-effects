#pragma once

#include "Effect.h"

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more calm fire.
#define SPARKLING 240

#define NUM_SPARKS 30

#define BRIGHTNESS 255
#define MIN_BRIGHTNESS 2

// The length of an "animation"
// The total length of animation can be calculated as ANIMATION_LENGTH/FRAMES_PER_UNIT
#define ANIMATION_LENGTH 4

class Spark : public Effect
{
protected:
  byte sparks[NUM_SPARKS];
  byte progress[NUM_SPARKS];
  //byte heat[NUM_LEDS];
  //float animation[ANIMATION_LENGTH];

  //void setHeatValue(int y, int value);
  void newSpark(void);
  //void Fire2022(void);
  void processSparks(void);
  void init(void);
public:
  Spark(int numPixels, EOrder colorOrder) : Effect(numPixels, colorOrder) {};
  void keepAlive(void);
};
