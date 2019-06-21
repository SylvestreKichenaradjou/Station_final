#include <Adafruit_AM2315.h>
#include <Wire.h>  //ajout de la librairie I2C
#include "RTClib.h"  // Bibliothèque pour le module RTC
#include <SoftwareSerial.h>
#define disk1 0x50    //Address of 24LC256 eeprom chip

unsigned int address = 0;
unsigned int address1 = 1;
unsigned int address2 = 2;
unsigned int address3 = 3;
unsigned int address4 = 4;

SoftwareSerial mavoieserie(11, 10); // (RX, TX) (pin Rx BT, pin Tx BT)
RTC_DS1307 rtc;      // module RTC de type DS1307
Adafruit_AM2315 am2315; // déclaration du capteur
char daysOfTheWeek[7][12] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

void setup()
{
  Serial.begin(9600);  //initialisation
  Wire.begin(); // initialisation de la liaison I2C
  mavoieserie.begin(9600);
}

void loop()
{
  //horloge();
  //Temp_Card();
  //girouette();
  mavoieserie.println(Temp_Ext() + Temp_Card() + horloge());
  delay(2000);
}

String  Temp_Ext ()
{
  float temperature = am2315.readTemperature();
  float humidite =  am2315.readHumidity();
  String celsius = "Température :/";
  String pourcent = " Humidité: /";
  String add = String();
  writeEEPROM(disk1, address1, am2315.readTemperature());
  writeEEPROM(disk1, address, am2315.readHumidity());
  add = celsius + temperature + "/" + pourcent + humidite + "/";
  Serial.println(add);
  delay(1000);
  return add;

}

String Temp_Card()
{

  int valeur_brute = analogRead(A0); // recupération de la tension sur la broche A0
  float temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0); // transforme la mesure en température en degrès Celcuis
  String temp_carte = " Température carte: /";
  String add = String();
  writeEEPROM(disk1, address2, temperature_celcius);
  add = temp_carte + temperature_celcius + "/";
  Serial.println(add);
  delay(1000);
  return add;


}
String horloge()
{
  String heure = "Date: /";
  String add = String();
  rtc.begin();  //initialisation du module horloge
  DateTime now = rtc.now();
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);//affiche le jour dela semaine
  Serial.print(" ");
  /* Serial.println(now.day() + now.month() + now.year() + now.hour() + now.minute() + now.second());
    int date = now.day() + now.month() + now.year() + now.hour() + now.minute() + now.second();
    writeEEPROM(disk1, address4, date);
    add = heure + date + "/";
    Serial.println(add);
    delay(1000);
    return add;*/

  Serial.print(now.day(), DEC); //affiche le jour+
  Serial.print("/");
  Serial.print(now.month(), DEC); //affiche le mois
  Serial.print("/");
  Serial.print(now.year(), DEC); //affice l'année
  Serial.print(" ");
  Serial.print(now.hour(), DEC); //affiche l'heure
  Serial.print(':');
  Serial.print(now.minute(), DEC); // affiche les minutes
  Serial.print(':');
  Serial.print(now.second(), DEC); //affiche les secondes
  Serial.println();
  delay(1000); //affiche la date toute les secondes
}

String girouette () {

  int val_girouette = 0;
  int codeur_g = A1;
  String girouette = "Direction: /";
  String add = String();
  val_girouette = analogRead(codeur_g);

  if (val_girouette >= 700 && val_girouette <= 824)
  {
    Serial.print("Nord = ");
  }
  if (val_girouette >= 407 && val_girouette <= 470)
  {
    Serial.print("Nord_est = ");
  }
  if (val_girouette >= 1 && val_girouette <= 115 )
  {
    Serial.print("Est = ");
  }
  if (val_girouette >= 116 && val_girouette <= 225)
  {
    Serial.print("Sud_est = ");
  }
  if (val_girouette >= 226 && val_girouette <= 320)
  {
    Serial.print("Sud = ");
  }
  if (val_girouette >= 600 && val_girouette <= 650)
  {
    Serial.print("Sud_Ouest = ");
  }
  if (val_girouette >= 900 && val_girouette <= 1022)
  {
    Serial.print("Ouest = ");
  }
  if (val_girouette >= 825 && val_girouette <= 899)
  {
    Serial.print("Nord_Ouest = ");
  }

  writeEEPROM(disk1, address3, val_girouette);
  add = girouette + val_girouette + "/";
  Serial.println(add);
  delay(1000);
  return add;
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data )
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

  delay(2000);
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
