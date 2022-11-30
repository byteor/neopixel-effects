#include "Fire.h"
#include "Spark.h"


NeoPixel_t strip(NUM_LEDS, LED_PIN);

//Fire fire(&strip);
//Spark spark(&strip);
Effect *effect;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");

  effect = new Fire(&strip);
  
  effect->setup();
}

void loop()
{
  effect->loop();
}