#include <Adafruit_AM2315.h>  //ajout de la librairie du capteur 
#include <Wire.h>  //ajout de la librairie I2C
#include "RTClib.h"  // Bibliothèque pour le module RTC
#include <SoftwareSerial.h>
#include <pt.h> //protothread
#define disk1 0x50    //Address of 24LC256 eeprom chip
#include <math.h>
#define WindSensorPin (7) // Digital Pin

static struct pt threadHorloge,threadTempCard,threadTempExt,threadGirouette,threadVent;

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

void setup()
{

  Serial.begin(9600);  //initialisation
  Wire.begin(); // initialisation de la liaison I2C
  mavoieserie.begin(9600);
  pinMode(WindSensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(WindSensorPin), isr_rotation, FALLING);

  
  PT_INIT(&threadHorloge);
  PT_INIT(&threadTempCard);
  PT_INIT(&threadTempExt);
  PT_INIT(&threadGirouette);
  PT_INIT(&threadVent);


}

void loop()
{

protothread1(&threadHorloge,10000);
protothread2(&threadTempCard,10000);
protothread3(&threadTempExt,10000);
protothread4(&threadGirouette,10000);
protothread5(&threadVent,10000);
interrupts();
  
}

static int protothread5(struct pt *pt,int interval)
{
  static unsigned long timestamp=0;
    interrupts(); // Enables interrupts


   PT_BEGIN(pt);

  while(1){

  PT_WAIT_UNTIL(pt,millis()-timestamp>interval);
  timestamp=millis();

  Rotations = 0;
  float WindSpeed;
  float vitesse;

  String vitesse_ = "Vitesse :/";
  String vit = String();


   interrupts();
   
  vitesse = Rotations / 5 * 2.4;

  delay(500);
  writeEEPROM(disk1, address3, vitesse);

  vit=vitesse;
  Serial.println("/V"+vit+"./");
  mavoieserie.println("/V"+vit+"./");
 
  }
  PT_END(pt);
}

static int protothread4 (struct pt *pt, int interval)
{
  static unsigned long timestamp=0;

  PT_BEGIN(pt);

  while(1){

  PT_WAIT_UNTIL(pt,millis()-timestamp>interval);
  timestamp=millis();

  
  int val_girouette = 0;
  int codeur_g = A1;
  
  String gir = String();
  String resultat;
  
  val_girouette = analogRead(codeur_g);

 if (val_girouette >= 81 && val_girouette <= 90 )
  {
    resultat = "Est ";
  }
  if (val_girouette >= 371 && val_girouette <= 663)
  {
    resultat = "Nord ";
  }
  if (val_girouette >= 370 && val_girouette <= 416)
  {
    resultat = "Nord Est ";
  }
  if (val_girouette >= 122 && val_girouette <= 176)
  {
    resultat = "Sud Est ";
  }
  if (val_girouette >= 177 && val_girouette <= 268)
  {
    resultat = "Sud " ;
  }
  if (val_girouette >= 526 && val_girouette <= 549)
  {
    resultat = "Sud Ouest ";
  }
  if (val_girouette >= 691 && val_girouette <= 771)
  {
    resultat = "Ouest ";
  }
  if (val_girouette >= 772 && val_girouette <= 880)
  {
    resultat = "Nord Ouest ";
  }

  gir=val_girouette;
  writeEEPROM(disk1, address3, val_girouette);
  Serial.println("/G"+gir+"./");
  Serial.println("/O"+resultat+"./");

  mavoieserie.println("/G"+gir+"./");
  mavoieserie.println("/O"+resultat+"./");
  
  }
  PT_END(pt);
}



static int protothread3 (struct pt *pt, int interval)
{
  static unsigned long timestamp=0;

  PT_BEGIN(pt);

  while(1){

  PT_WAIT_UNTIL(pt,millis()-timestamp>interval);
  timestamp=millis();

  
  float temperature = am2315.readTemperature();
  float humidite =  am2315.readHumidity();

  String temp = String();
  String hum=String();
  writeEEPROM(disk1, address1, am2315.readTemperature());
  writeEEPROM(disk1, address, am2315.readHumidity());
  temp=  temperature ;
  hum=humidite;
  Serial.println("/TE"+temp+"./");
  Serial.println("/H"+hum+"./");
  mavoieserie.println("/TE"+temp+"./");
  mavoieserie.println("/H"+hum+"./");
  }
  PT_END(pt);
}

static int protothread2(struct pt *pt,int interval)
{
  static unsigned long timestamp=0;

  PT_BEGIN(pt);

  while(1){

  PT_WAIT_UNTIL(pt,millis()-timestamp>interval);
  timestamp=millis();
  
  int valeur_brute = analogRead(A0); // recupération de la tension sur la broche A0
  float temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0); // transforme la mesure en température en degrès Celcuis
  String temp = String();
  writeEEPROM(disk1, address2, temperature_celcius);
  temp=  temperature_celcius ;
  Serial.println("/TC"+temp+"./");  
  mavoieserie.println("/TC"+temp+"./");

  
  }
  PT_END(pt);
}


static int protothread1(struct pt *pt,int interval)
{

 static unsigned long timestamp=0;

 PT_BEGIN(pt);

  while(1){
 

  PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis();
    
  rtc.begin();  //initialisation du module horloge
  DateTime now = rtc.now();


  Serial.print("/D ");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);//affiche le jour dela semaine
  Serial.print(" ");
  Serial.print(now.day(), DEC); //affiche le jour
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
  Serial.print(" ./");
  Serial.print("\n \r");
  
  mavoieserie.print("/D ");
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
  mavoieserie.print(" ./");
  mavoieserie.print("\n \r");

  }

  PT_END(pt);
}


void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data )
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

 // delay(5);
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

void isr_rotation() {

  volatile unsigned long ContactBounceTime;
  if ((millis() - ContactBounceTime) > 15 ) { // debounce the switch contact.
    Rotations++;
    ContactBounceTime = millis();
  }

}
