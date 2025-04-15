#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DS3231.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 4 // Port andem die Temperatursensoren angeschlossen sind
DS3231 rtc(SDA, SCL);

File DataStorage; // Definieren von DataStorage als Datei

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int delayamount = 10000; // Abstand zwischen den wertespeicherungen in ms
int deviceCount = 0;
float tempC;
unsigned long letzteZeit = 0;
int CS_PIN = 10; // Port andem der CS pin des SD-Karten gerätes angeschlossen ist


void setup() {
  Serial.begin(115200); // Starten der senosoren 
  Wire.begin();
  rtc.begin();
  sensors.begin();

  deviceCount = sensors.getDeviceCount(); // Variable anzahl der Temperatur - Sensoren
  Serial.print("Found ");
  Serial.print(deviceCount, DEC); // Zählen der angeschlossenen Temperatursensoren
  Serial.println(" devices.");
  Serial.println("");

  if (SD.begin()){ // Testen der SD - Karte
    Serial.println("SD karte bereit");
  } else {
    Serial.println("SD karte nicht erkannt");
  }

  
}

void loop() {
 

  unsigned long startZeit = millis();
   
  sensors.requestTemperatures(); // Abrufen der Werte von den Sensoren
   String aktuellesDatum = rtc.getDateStr(); // Abrufen des aktuellen Datums
   aktuellesDatum.replace(".",""); // ersetzen von . im datum zu - Neues Format DD-MM-YYYY
   aktuellesDatum = aktuellesDatum + ".txt";
   
   DataStorage = SD.open(aktuellesDatum, FILE_WRITE); // öffnen/erstellen einer Datei Name: Datum
   if (DataStorage) {
    float Temp1 = sensors.getTempCByIndex(0); // Variable mit Temperaturen erstellen
    float Temp2 = sensors.getTempCByIndex(1);
    float Temp3 = sensors.getTempCByIndex(2);
    
    String Temp1komma = String(Temp1, 2); // Umwandeln der Werte in String
    String Temp2komma = String(Temp2, 2);
    String Temp3komma = String(Temp3, 2);
    
    Temp1komma.replace('.', ','); // Ersetzen der . durch ,
    Temp2komma.replace('.', ',');
    Temp3komma.replace('.', ',');
    
    DataStorage.print(rtc.getTimeStr()); // Schreiben der Werte auf die SD-Karte in Datei DD-MM-YYYY.txt
    DataStorage.print("; ");
    DataStorage.print(Temp1komma);
    DataStorage.print("; ");
    DataStorage.print(Temp2komma);
    DataStorage.print("; ");
    DataStorage.println(Temp3komma);
    DataStorage.close(); // schließen der Datei

    Serial.println("Daten Punkt auf SD-Karte gespeichert"); // Seriell Monitor ausgabe 
   } else {
    Serial.print(aktuellesDatum);
    Serial.println("Datei konnte nicht geöffnet werden"); // Fehlermeldung wenn datei öffnung fehlgeschlagen
   }

delay(delayamount); // verzögerung um variable oben eingestellt









   

}
