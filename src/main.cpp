#include "Fire.h"
#include "Spark.h"
#include "Wonder.h"


//NeoPixel_t strip(NUM_LEDS, LED_PIN);

//Fire fire(&strip);
//Spark spark(&strip);
Effect *effect;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");

  effect = new Wonder(100, EOrder::GRB);
  //effect = new Spark(60, EOrder::RGB);
  

  effect->setup();
}

void loop()
{
  effect->loop();
}