#include "Wonder.h"

void Wonder::init(void)
{
    Serial.println("Wonder...");

    WonderStep chase1 = WonderStep(WonderEffect::CHASE_UP);
    WonderStep chase2 = WonderStep(WonderEffect::CHASE_DOWN);
    WonderStep bright1 = WonderStep(WonderEffect::INCREASE_BRIGHTNESS, 0, 32);
    WonderStep bright2 = WonderStep(WonderEffect::INCREASE_BRIGHTNESS, 32, 255);
    WonderStep bright3 = WonderStep(WonderEffect::INCREASE_BRIGHTNESS, 0, 10);
    WonderStep bright4 = WonderStep(WonderEffect::INCREASE_BRIGHTNESS, 100, 255);
    WonderStep dark1 = WonderStep(WonderEffect::DECREASE_BRIGHTNESS, 255, 32);
    WonderStep dark2 = WonderStep(WonderEffect::DECREASE_BRIGHTNESS, 32, 0);
    WonderStep dark3 = WonderStep(WonderEffect::DECREASE_BRIGHTNESS, 32, 7);
    WonderStep dark4 = WonderStep(WonderEffect::DECREASE_BRIGHTNESS, 255, 100);
    WonderStep sparkling = WonderStep(WonderEffect::SPARKLE);

    // 4 * 8 + 1

    RgbColor color0 = CRGB2RgbColor(CRGB::Black);
    RgbColor color1 = CRGB2RgbColor(CRGB::LawnGreen);
    RgbColor color2 = CRGB2RgbColor(CRGB::Blue);
    RgbColor color3 = CRGB2RgbColor(CRGB::Red);
    RgbColor color4 = CRGB2RgbColor(CRGB::Yellow);
    RgbColor color5 = CRGB2RgbColor(CRGB::White);

    WonderStep transition0 = WonderStep(WonderEffect::BACKGROUND, color0, color0);
    WonderStep transition1 = WonderStep(WonderEffect::BACKGROUND, color0, color1);
    WonderStep transition2 = WonderStep(WonderEffect::BACKGROUND, color1, color2);
    WonderStep transition3 = WonderStep(WonderEffect::BACKGROUND, color2, color3);
    WonderStep transition4 = WonderStep(WonderEffect::BACKGROUND, color3, color4);

    WonderStep transition5 = WonderStep(WonderEffect::BACKGROUND, color4, color5);
    WonderStep transition6 = WonderStep(WonderEffect::BACKGROUND, color1, color4);

    WonderStep transition7 = WonderStep(WonderEffect::BACKGROUND, color4.Dim(200), color1.Dim(200));

    steps[0][0] = chase1;
    steps[0][1] = transition0;

    steps[1][0] = chase1;

    steps[2][0] = chase1;
    steps[2][1] = bright1;
    steps[2][2] = transition1;

    // steps[3][0] = chase1;
    steps[3][0] = bright2;
    
    steps[4][0] = transition2;    
    steps[5][0] = dark4;
    steps[6][0] = transition3;
    steps[6][1] = bright4;
    //steps[7][0] = transition6;
    steps[7][1] = dark4;
    
    steps[8][0] = transition4;
    steps[8][1] = dark1;

    steps[9][0] = transition5;
    steps[9][1] = dark3;

    steps[10][0] = sparkling;

    totalSteps = 11;
    setStep(0);

    CRGB baseColor = CRGB(100, 100, 100);

    for (int i = 0; i < numChases; i++)
    {
        CRGB color = baseColor;
        color.r += random8(200) - 55;
        color.g += random8(200) - 55;
        color.b += random8(200) - 55;
        chaseColor[i] = CRGB2RgbColor(color);
        Serial.printf("Chase [%d] = [%d, %d, %d]\n", i, color.r, color.g, color.b);
        // chaseY[i] = -chaseGap();
    }

    chaseColor[0] = RgbColor(200, 0, 0);
    chaseColor[1] = RgbColor(0, 200, 0);
    chaseColor[2] = RgbColor(0, 0, 200);
    chaseColor[3] = RgbColor(0, 100, 150);
    chaseColor[4] = RgbColor(150, 0, 200);
    chaseColor[5] = RgbColor(150, 150, 0);
}

void Wonder::processLights()
{
    effectBrightness();
    effectBackground();
    effectChase();
    effectSparks();
}

void Wonder::effectBackground()
{
    for (int i = 0; i < MAX_PARALLEL_STEPS; i++)
    {
        if (steps[currentStep][i].effect == WonderEffect::BACKGROUND)
        {
            bgColor = RgbColor::LinearBlend(steps[currentStep][i].color1, steps[currentStep][i].color2, (float)(currentStepProgress + 1) / (float)framesPerTimeUnit);
            break;
        }
    }
}

void Wonder::effectBrightness()
{
    for (int i = 0; i < MAX_PARALLEL_STEPS; i++)
    {
        if (steps[currentStep][i].effect == WonderEffect::INCREASE_BRIGHTNESS || steps[currentStep][i].effect == WonderEffect::DECREASE_BRIGHTNESS)
        {
            brightness = steps[currentStep][i].startingBrightness + (float)(steps[currentStep][i].endingBrightness - steps[currentStep][i].startingBrightness) * (float)(currentStepProgress + 1) / (float)framesPerTimeUnit;
            // Serial.printf("=== %d, %d / %d\n", brightness, currentStepProgress, framesPerTimeUnit);
            break;
        }
    }
}

void Wonder::newSpark()
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

void Wonder::effectSparks()
{
    for (int i = 0; i < NUM_SPARKS; i++)
    {
        if (progress[i] > 0)
        {
            progress[i]--;
            leds[sparks[i]] = RgbColor((byte)(MIN_BRIGHTNESS + progress[i] * (DEFAULT_BRIGHTNESS - MIN_BRIGHTNESS) / ANIMATION_LENGTH));
            if (progress[i] <= 0)
                progress[i] = 0;
        }
    }

    for (int i = 0; i < MAX_PARALLEL_STEPS; i++)
    {
        if (steps[currentStep][i].effect == WonderEffect::SPARKLE && random8() < SPARKLING)
        {
            newSpark();
            break;
        }
    }
}

void Wonder::effectChase()
{
    bool isChaseActive = false;
    for (int i = 0; i < MAX_PARALLEL_STEPS; i++)
    {
        isChaseActive |= steps[currentStep][i].effect == WonderEffect::CHASE_UP || steps[currentStep][i].effect == WonderEffect::CHASE_DOWN;
    }

    int minY = numLeds;
    int maxY = -1;
    for (int i = 0; i < numChases; i++)
    {
        if (chases[i].active)
        {
            int y = chases[i].y;
            if (minY > y)
                minY = y;
            if (maxY < y)
                maxY = y;

            if (y >= 0 && y < numLeds)
            {
                leds[y] = chaseColor[i];
                // Serial.printf("[%d]=%d of %d\n", y, leds[y].B, chaseColor[i].B);
            }
            chases[i].y += chaseUp ? chaseSpeed : -chaseSpeed;
            if (chaseUp && chases[i].y > numLeds || !chaseUp && chases[i].y < 0)
                chases[i].active = false;
        }
    }
    if (isChaseActive)
        if (chaseUp && minY >= chaseGap() - chaseSpeed || !chaseUp && maxY <= numLeds - chaseGap() - 1 + chaseSpeed)
            for (int i = 0; i < numChases; i++)
            {
                if (!chases[i].active)
                {
                    chases[i].active = true;
                    chases[i].y = chaseUp ? 0 : numLeds - 1;
                    Serial.print("+");
                    // Serial.print(minY);
                    break;
                }
            }
}

void Wonder::processSteps(void)
{
    currentStepProgress++;
    if (currentStepProgress >= framesPerTimeUnit)
    {
        Serial.print(currentStepProgress);
        currentStepProgress = 0;
        currentStep++;
        if (currentStep >= totalSteps)
        {
            currentStep = totalSteps - 1; // play the last one forever
            //currentStep = 0;              // TODO: remove this line - for debugging only!
        }
        setStep(currentStep);
    }
}

void Wonder::setStep(int step)
{
    currentStepProgress = 0;
    if (step >= totalSteps)
    {
        currentStep = totalSteps - 1; // play the last one forever
    }
    Serial.print(":");
    Serial.print(currentStep);
    for (int i = 0; i < MAX_PARALLEL_STEPS; i++)
    {
        switch (steps[currentStep][i].effect)
        {
        case WonderEffect::CHASE_UP:
            chaseUp = true;
            Serial.print("-^-");
            break;
        case WonderEffect::CHASE_DOWN:
            chaseUp = false;
            Serial.print("-v-");
            break;
        case WonderEffect::INCREASE_BRIGHTNESS:
            Serial.print("-*-");
            break;
        case WonderEffect::DECREASE_BRIGHTNESS:
            Serial.print("-o-");
            break;
        case WonderEffect::SPARKLE:
            Serial.print("-x-");
            break;
        }
    }
}

void Wonder::processRandomness()
{
}

void Wonder::keepAlive(void)
{
    for (int i = 0; i < numLeds; i++)
    {
        leds[i] = bgColor.Dim(brightness);
    }
    // Step 1. Process
    processLights();

    // Step 2. Progress through steps
    processSteps();

    // Step 3.  Do random stuff
    processRandomness();
}