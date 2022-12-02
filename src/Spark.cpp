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
            leds[sparks[i]] = RgbColor((byte) (MIN_BRIGHTNESS + progress[i] * (BRIGHTNESS - MIN_BRIGHTNESS) / ANIMATION_LENGTH));
            if(progress[i] <= 0) progress[i] = 0;
        }
    }
}

void Spark::newSpark()
{
    int y = random16(numLeds);
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
    for (int i = 0; i < numLeds; i++)
    {
        leds[i] = RgbColor(MIN_BRIGHTNESS);
    }
    // Step 2. Process
    processSparks();

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKLING)
    {
        newSpark();
    }
}