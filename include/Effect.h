#pragma once

#include <NeoPixelBus.h>
#include <FastLED.h>

typedef NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> NeoPixel_t;

#define FRAMES_PER_SECOND 30

#define NUM_LEDS 300 // 60

// Some strips have Blue and Green messed up
#define SWAP_BLUE_AND_GREEN false

// make sure to set this to the correct pin, ignored for Esp8266
#define LED_PIN 3

// NOTE: These will ignore the LED_PIN and use GPI03 pin
//extern NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(NUM_LEDS, LED_PIN);

class Effect
{
protected:
    long timer;
    CRGB leds[NUM_LEDS];
    NeoPixel_t *strip;
    virtual void init(void) {};
public:
    Effect(NeoPixel_t *st) 
    {
        this->strip = st;
    }
    void setup(void);
    void loop(void);
    virtual void keepAlive(void);
};
