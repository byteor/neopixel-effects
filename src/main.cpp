#include "Fire.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Fire2022: Hot Coal");
  setupFire();
}

void loop()
{
  keepFireAlive();
}