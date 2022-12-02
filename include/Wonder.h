#pragma once

#include "Effect.h"

/**
 * "WONDER" effect
 *
 *
 */

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more calm fire.
// #define SPARKLING 240
// #define NUM_SPARKS 30

// Chase lights
#define MAX_NUM_CHASES 5
#define DEFAULT_CHASE_WIDTH 3
// move per frame
#define DEFAULT_CHASE_SPEED 2

#define NUM_SPARKS 30
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more calm fire.
#define SPARKLING 240
// Spark animation length
#define ANIMATION_LENGTH 4
#define MIN_BRIGHTNESS 2

// The length of a cycle, ms
//#define STEP_LENGTH 265
#define MAX_STEPS 32
#define MAX_PARALLEL_STEPS 3

enum WonderEffect
{
    NONE = 0,
    CHASE_UP = 1,
    CHASE_DOWN = 2,
    INCREASE_BRIGHTNESS = 4,
    DECREASE_BRIGHTNESS = 8,
    SPARKLE = 16,
    BACKGROUND = 32
};

struct WonderStep
{
    WonderEffect effect = NONE;
    RgbColor color1;
    RgbColor color2;
    int startingBrightness;
    int endingBrightness;

    WonderStep()
    {
    }
    WonderStep(WonderEffect effect, int start, int finish)
    {
        this->effect = effect;
        this->startingBrightness = start;
        this->endingBrightness = finish;
    }
    WonderStep(WonderEffect effect)
    {
        this->effect = effect;
    }
    WonderStep(WonderEffect effect, RgbColor bg1, RgbColor bg2)
    {
        this->effect = effect;
        this->color1 = bg1;
        this->color2 = bg2;
    }
};

struct Chase
{
    bool active = false;
    int y = -1;
    RgbColor color;
};

class Wonder : public Effect
{

protected:
    WonderStep steps[MAX_STEPS][MAX_PARALLEL_STEPS];
    int currentStep = 0;
    int currentStepProgress = 0;
    int totalSteps = 0;

    // Chase effect
    int numChases = MAX_NUM_CHASES;
    int chaseWidth = DEFAULT_CHASE_WIDTH;
    // the distance between chases
    int chaseGap() { return numLeds / numChases; }
    int chaseSpeed = DEFAULT_CHASE_SPEED;
    Chase chases[MAX_NUM_CHASES];
    RgbColor chaseColor[MAX_NUM_CHASES];
    bool chaseUp;
    void effectChase(void);

    // background effect
    RgbColor bgColor;
    void effectBackground();
    void effectBrightness(void);

    // Sparks
  byte sparks[NUM_SPARKS];
  byte progress[NUM_SPARKS];
    void newSpark(void);
    void effectSparks(void);

    // light effects
    void processLights(void);

    void processSteps(void);
    void setStep(int step);
    void processRandomness(void);
    void init(void);

public:
    Wonder(int numPixels, EOrder colorOrder) : Effect(numPixels, colorOrder){};
    void keepAlive(void);
};
