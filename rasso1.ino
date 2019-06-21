



#include <Adafruit_AM2315.h>
#include <Wire.h>  //ajout de la librairie I2C
#include "RTClib.h"  // Bibliothèque pour le module RTC
#include <SoftwareSerial.h>
#define disk1 0x50    //Address of 24LC256 eeprom chip
#include <math.h>
#define WindSensorPin (7) // Digital Pin

unsigned int address = 0;
unsigned int address1 = 1;
unsigned int address2 = 2;
unsigned int address3 = 3;
unsigned int address4 = 4;
volatile unsigned long Rotations;

SoftwareSerial mavoieserie(11, 10); // (RX, TX) (pin Rx BT, pin Tx BT)   
RTC_DS1307 rtc;      // module RTC de type DS1307
Adafruit_AM2315 am2315; // déclaration du capteur
char daysOfTheWeek[7][12] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

void loop()
{
 //interrupts(); // Enables interrupts
  horloge();
  //Temp_Card();
 // Temp_Ext();
 // Vent();
 // Direction();
  //mavoieserie.println(Temp_Ext() + Temp_Card());
  delay(1000);
}

String  Temp_Ext ()
{
  float temperature = am2315.readTemperature();
  float humidite =  am2315.readHumidity();
  String celsius = "Température :/";
  String pourcent = " Humidité: /";
  String add = String();
 // writeEEPROM(disk1, address1, am2315.readTemperature());
 // writeEEPROM(disk1, address, am2315.readHumidity());
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
  //writeEEPROM(disk1, address2, temperature_celcius);
  add = temp_carte + temperature_celcius + "/";
  Serial.println(add);
  delay(1000);
  return add;


}
void horloge()
{
  rtc.begin();  //initialisation du module horloge
  DateTime now = rtc.now();
  mavoieserie.print(daysOfTheWeek[now.dayOfTheWeek()]);//affiche le jour dela semaine
 mavoieserie.print(" ");
  mavoieserie.print(now.day(), DEC); //affiche le jour
  mavoieserie.print("/");
  mavoieserie.print(now.month(), DEC); //affiche le mois
  mavoieserie.print("/");
  mavoieserie.print(now.year(), DEC); //affice l'année
  mavoieserie.print(" ");
  mavoieserie.print(now.hour(), DEC); //affiche l'heure
  mavoieserie.print(':');
  mavoieserie.print(now.minute(), DEC); // affiche les minutes
  mavoieserie.print(':');
  mavoieserie.print(now.second(), DEC); //affiche les secondes
  Serial.println();
  delay(1000); //affiche la date toute les secondes
}

String Direction()
{
  int val_girouette = 0;
  int codeur_g = A1;

  val_girouette = analogRead(codeur_g);
  String direction_ = "Direction :/";
  String resultat;
  String add;
  Serial.println(val_girouette);
  if (val_girouette >= 81 && val_girouette <= 90 )
  {
    resultat ="Est/";
  }
  if (val_girouette >= 371 && val_girouette <= 663)
  {
    resultat = "Nord/";
  }
  if (val_girouette >= 370 && val_girouette <= 416)
  {
    resultat = "Nord_est/";
  }
  if (val_girouette >= 122 && val_girouette <= 176)
  {
    resultat = "Sud_est/";
  }
  if (val_girouette >= 177 && val_girouette <= 268)
  {
    resultat ="Sud/" ;
  }
  if (val_girouette >= 526 && val_girouette <= 549)
  {
    resultat ="Sud_Ouest/";
  }
  if (val_girouette >= 691 && val_girouette <= 771)
  {
    resultat ="Ouest/";
  }
  if (val_girouette >= 692 && val_girouette <= 733)
  {
    resultat ="Nord_Ouest/";
  }
  add = direction_ + resultat;
  Serial.println(add);
  delay(1000);
  return add;
}


String Vent ()
{
  Rotations = 0;
  float WindSpeed;
  float Vitesse;

  String vitesse_ = "Vitesse :/";
  String add = String();
 
  delay (1000); // Wait 1 seconds to average
 
  Vitesse = Rotations/5 *2.4;
  //Serial.print(Vitesse);
  //Serial.print("/");
  delay(500);
  //writeEEPROM(disk1, address3, val_girouette);
  add = vitesse_ + Vitesse + "kmh";
  Serial.println(add);
  delay(1000);
  return add;
}

void isr_rotation() {

  volatile unsigned long ContactBounceTime;
  if ((millis() - ContactBounceTime) > 15 ) { // debounce the switch contact.
    Rotations++;
    ContactBounceTime = millis();
  }
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
