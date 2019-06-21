#include "Wire.h"
#include <EEPROM.h>
uint8_t EPROM1 = 0x50;
float value;
void setup() {
  Wire.begin();
  Serial.begin(9600); // initialisation de la communication avec le pc
  
}

void loop() {
  
  int valeur_brute = analogRead(A0); // recupération de la tension sur la broche A0
  float temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0); // transforme la mesure en température en degrès Celcuis
  //Serial.println(temperature_celcius); //affichage de la température toutes les secondes
  delay(1000);
}
