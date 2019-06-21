#include <Wire.h>

#define disk1 0x50    //Address of 24LC256 eeprom chip

unsigned int address = 0;

void setup(void)
{
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  int valeur_brute = analogRead(A0); // recupération de la tension sur la broche A0
  float temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0); // transforme la mesure en température en degrès Celcuis
  writeEEPROM(disk1, address, temperature_celcius);  
  Serial.print("Température carte: "); Serial.println(readEEPROM(disk1, address), DEC);
  delay(1000);
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data )
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

  delay(5);
}

byte readEEPROM(int deviceaddress, unsigned int eeaddress )
{
  byte rdata = 0xFF;

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(deviceaddress, 1);

  if (Wire.available()) rdata = Wire.read();

  return rdata;
}
