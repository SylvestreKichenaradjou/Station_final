#include <Adafruit_AM2315.h>
#include <Wire.h>
#define disk1 0x50    //Address of 24LC256 eeprom chip

unsigned int address = 0;
unsigned int address1 = 1;

Adafruit_AM2315 am2315;

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Test Capteur");

  if (! am2315.begin()) {
     Serial.println("Capteur non trouvé, vérifier le cablâge et les résistances");
     while (1);
  }
}

void loop() {
  float temp = am2315.readTemperature();
  float humidity = am2315.readHumidity();
  //Serial.print("Humidité: "); Serial.println(humidity);
  //Serial.print("Température: "); Serial.println(am2315.readTemperature());
  delay(1000);
  writeEEPROM(disk1, address1, temp);
  writeEEPROM(disk1, address, humidity);
  Serial.print("Température: "); Serial.println(readEEPROM(disk1, address1), DEC);
  Serial.print("Humidité: "); Serial.println(readEEPROM(disk1, address), DEC);
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
