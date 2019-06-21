#include <Wire.h>  //ajout de la librairie pour l'I2C
#include "RTClib.h" // ajout de la librairie pour l'horloge 

RTC_DS1307 rtc;  //declaration du module horloge

char daysOfTheWeek[7][12] = {"Dimanche","Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

void setup () {
  Serial.begin(9600);
  rtc.begin();
  delay(100);
}

void loop () {
    DateTime now = rtc.now();
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
    Serial.println();
    
    delay(2000); //affiche la date toute les 2 secondes 
}
