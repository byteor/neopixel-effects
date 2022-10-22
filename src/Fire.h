#ifndef _FIRE_H_
#define _FIRE_H_

#include <NeoPixelBus.h>
#include <FastLED.h>

struct Flame
{
  int y;
  byte width;
  int heat;
  byte speed;
  byte progress;
};

void setupFire(void);
void keepFireAlive(void);
void setHeatValue(int y, int value);
void newFlame(void);
void Fire2022(void);
void processFlames(void);

#endif //_FIRE_H_