#include "Spark.h"

void Spark::init(void)
{
    Serial.println("Sparks...");
}

void Spark::processSparks()
{
    for (int i = 0; i < NUM_SPARKS; i++)
    {
        if (progress[i] > 0)
        {
            progress[i]--;
            heat[sparks[i]] = (byte) (MIN_BRIGHTNESS + progress[i] * (BRIGHTNESS - MIN_BRIGHTNESS) / ANIMATION_LENGTH);
            if(progress[i] <= 0) progress[i] = 0;
        }
    }
}

void Spark::newSpark()
{
    int y = random16(NUM_LEDS);
    for (int i = 0; i < NUM_SPARKS; i++)
    {
        if (progress[i] == 0)
        {
            sparks[i] = y;
            progress[i] = ANIMATION_LENGTH + 1;
            return;
        }
    }
}

void Spark::keepAlive(void)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        heat[i] = MIN_BRIGHTNESS;
    }
    // Step 2. Process
    processSparks();

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKLING)
    {
        newSpark();
    }
    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < NUM_LEDS; j++)
    {
        CRGB color = HeatColor(heat[j]);
        color.setRGB(heat[j], heat[j], heat[j]);
        leds[j] = color;
    }
}