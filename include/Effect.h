#pragma once

#include <NeoPixelBus.h>
#include <FastLED.h>

typedef NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> NeoPixel_t;

#define BASE_TIME_UNIT_MS 265 * 4 // Base time unit we count frames off
#define FRAMES_PER_UNIT 20        // 33 is the best fit for Wonder effect

#define NUM_LEDS 100 // 300 // 60

#define MAX_NUM_LEDS 300
// make sure to set this to the correct pin, ignored for Esp8266
// NOTE: These will ignore the LED_PIN and use GPI03 pin
#define LED_PIN 3

#define DEFAULT_BRIGHTNESS 255
#define DEFAULT_MIN_BRIGHTNESS 20

class Effect
{
protected:
    int baseTimeUnitMs = BASE_TIME_UNIT_MS;
    int framesPerTimeUnit = FRAMES_PER_UNIT;
    int numLeds = NUM_LEDS;
    EOrder colorOrder = EOrder::RGB;
    uint8_t brightness = DEFAULT_BRIGHTNESS;
    uint8_t minBrightness = DEFAULT_MIN_BRIGHTNESS;

    long timer;
    RgbColor leds[MAX_NUM_LEDS];
    NeoPixel_t *strip = NULL;
    virtual void init(void){};
    RgbColor translateColor(RgbColor color);
    RgbColor CRGB2RgbColor(CRGB color);
public:
    Effect(int numPixels, EOrder colorOrder)
    {
        this->colorOrder = colorOrder;
        this->numLeds = numPixels;
    }

    void setTimeUnit(int timeUnitMs, int framesPerUnit)
    {
        this->baseTimeUnitMs = timeUnitMs;
        this->framesPerTimeUnit = framesPerTimeUnit;
    }

    void setNumLeds(int numberOfLeds)
    {
        numLeds = numberOfLeds;
    }

    void setup(void);
    void loop(void);
    virtual void keepAlive(void);
};
