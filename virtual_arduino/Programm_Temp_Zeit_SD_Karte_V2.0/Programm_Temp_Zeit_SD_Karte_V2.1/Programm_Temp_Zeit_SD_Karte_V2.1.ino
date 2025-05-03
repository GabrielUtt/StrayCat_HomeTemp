// TDs Ideas

// Remove array and max count and directly use sensors.getTempCByIndex(i) without intermediate save and string transformation

#include <SPI.h> // standard library to communicate with SPI devices (no installation)
#include <SD.h> // standard library that enables reading and writing on SD cards (no installation)	
#include <Wire.h> // standard library to communicate with I2C devices (no installation)
#include <DS3231.h> // install from http://www.rinkydinkelectronics.com/library.php?id=73
#include <DallasTemperature.h> //Arduino library for Dallas/Maxim temperature ICs (to be installed)
// Support for DS18B20 and other 1-Wire temperature sensors (to be installed)
#include <OneWire.h> // Access 1-wire temperature sensors, memory and other chips <to be installed)

#define ONE_WIRE_BUS 4 // Port andem die Temperatursensoren angeschlossen sind
DS3231 rtc(SDA, SCL); // Start an instance of the DS3231 class, using 
// Arduino UNO standard pins (SDA, SCL) for  I²C communication

File DataStorage; // Definieren von DataStorage als Datei

OneWire oneWire(ONE_WIRE_BUS); // create oneWire object
DallasTemperature sensors(&oneWire); // create DallasTemp object, referencing oneWire object

int delayamount = 10000; // Abstand zwischen den wertespeicherungen in ms
int deviceCount = 0;
unsigned long letzteZeit = 0;
int CS_PIN = 10; // Port andem der CS pin des SD-Karten Gerätes angeschlossen ist

const int MAX_SENSORS = 5; // maximum number of sensors expected
float Temp[MAX_SENSORS]; // fixed array for temperature values
String TempStr[MAX_SENSORS]; // fixed array for temperature strings

void setup() {
  Serial.begin(115200); // Starten der senosoren 
  Wire.begin();
  rtc.begin();
  sensors.begin();

  deviceCount = sensors.getDeviceCount(); // Variable anzahl der Temperatur - Sensoren

  if (deviceCount > MAX_SENSORS) {
    deviceCount = MAX_SENSORS; // limit to maximum to avoid array overflow
  }

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
   String aktuellesDatum = rtc.getDateStr(FORMAT_LONG, FORMAT_BIGENDIAN); // Abrufen des aktuellen Datums
   aktuellesDatum.replace(".",""); // Neues Format DDMMYYYY
   String CurrFileName = aktuellesDatum + ".txt";
   
   DataStorage = SD.open(CurrFileName, FILE_WRITE); // öffnen/erstellen einer Datei Name: Datum
   
   if (DataStorage) {
	for (int i = 0; i < deviceCount; i++) {
	  Temp[i] = sensors.getTempCByIndex(i); // store each temperature
	  TempStr[i] = String(Temp[i], 2);
	  TempStr[i].replace('.', ',');
	}
	
	
/*	float Temp1 = sensors.getTempCByIndex(0); // Variable mit Temperaturen erstellen
    float Temp2 = sensors.getTempCByIndex(1);
    float Temp3 = sensors.getTempCByIndex(2); 
    
    String Temp1komma = String(Temp1, 2); // Umwandeln der Werte in String
    String Temp2komma = String(Temp2, 2);
    String Temp3komma = String(Temp3, 2);
    
    Temp1komma.replace('.', ','); // Ersetzen der . durch ,
    Temp2komma.replace('.', ',');
    Temp3komma.replace('.', ','); */

  DataStorage.print(rtc.getDateStr(FORMAT_LONG, FORMAT_BIGENDIAN));
  DataStorage.print(";");
	DataStorage.print(rtc.getTimeStr()); // Print time

	for (int i = 0; i < deviceCount; i++) {
	  DataStorage.print(";");            // Print separator
	  DataStorage.print(TempStr[i]);      // Print temperature string
	}

	DataStorage.println(); // Finish the line (write newline)
	DataStorage.close(); // schließen der Datei 
	
  /*  DataStorage.print(rtc.getTimeStr()); // Schreiben der Werte auf die SD-Karte in Datei DD-MM-YYYY.txt
    DataStorage.print("; ");
    DataStorage.print(Temp1komma);
    DataStorage.print("; ");
    DataStorage.print(Temp2komma);
    DataStorage.print("; ");
    DataStorage.println(Temp3komma); */


    Serial.println("Daten Punkt auf SD-Karte gespeichert"); // Seriell Monitor ausgabe 
   } else {
    Serial.print(CurrFileName);
    Serial.println("Datei konnte nicht geöffnet werden"); // Fehlermeldung wenn datei öffnung fehlgeschlagen
   }

delay(delayamount); // verzögerung um variable oben eingestellt









   

}
