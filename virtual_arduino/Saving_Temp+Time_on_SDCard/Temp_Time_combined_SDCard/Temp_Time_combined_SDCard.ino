#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DS3231.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 4
DS3231 rtc(SDA, SCL);
File myFile;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int deviceCount = 0;
float tempC;

int CS_PIN = 10; // Bei Uno = 10, Mega = 53

String currentDateString = "";  // Zum Überwachen, ob sich das Datum ändert
unsigned long lastWriteTime = 0;
const unsigned long writeInterval = 10000; // z.B. alle 60 Sek. schreiben


void setup() {
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  sensors.begin();

  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  
 if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

void loop() {
  sensors.requestTemperatures();
   Serial.print(rtc.getTimeStr());
    Serial.print(",");    
    Serial.print(sensors.getTempCByIndex(0));
    Serial.print(", ");
    Serial.print(sensors.getTempCByIndex(1));
    Serial.print(", ");
    Serial.println(sensors.getTempCByIndex(2));
 
  myFile = SD.open("test3.txt", FILE_WRITE);
  if (myFile) {    
    myFile.print(rtc.getTimeStr());
    myFile.print(",");    
    myFile.print(sensors.getTempCByIndex(0));
    myFile.print(", ");
    myFile.print(sensors.getTempCByIndex(1));
    myFile.print(", ");
    myFile.println(sensors.getTempCByIndex(2));
    myFile.close(); // close the file
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");

    }
    delay(5000);
  }


// Holt aktuelles Datum im Format YYYY-MM-DD
String getDateString() {
  String rawDate = rtc.getDateStr(); // z. B. "13/04/2025"
  
  // Aufteilen in Tag, Monat, Jahr
  int day = rawDate.substring(0, 2).toInt();
  int month = rawDate.substring(3, 5).toInt();
  int year = rawDate.substring(6, 10).toInt();

  // Formatieren zu "YYYY-MM-DD"
  return String(year) + "-" + addLeadingZero(month) + "-" + addLeadingZero(day);
}

// Funktion zum Erstellen und Öffnen der Log-Datei
int createLogFile(String dateStr) {
  if (myFile) myFile.close(); // Schließe eventuell geöffnete Datei

  String filename = dateStr + ".txt";
  Serial.print("Öffne Datei: ");
  Serial.println(filename);

  // Teste, ob die Datei bereits existiert
  if (SD.exists(filename)) {
    Serial.println("Die Datei existiert bereits.");
  } else {
    Serial.println("Die Datei existiert nicht. Erstelle sie.");
  }

  // Öffnen der Datei im Schreibmodus
  myFile = SD.open(filename, FILE_WRITE);

  if (!myFile) {
    Serial.println("Fehler beim Öffnen oder Erstellen der Datei!");
    return 0; // Fehler beim Öffnen
  } else {
    // Falls die Datei erfolgreich geöffnet wurde, schreibe eine Header-Zeile
    myFile.println("=== Neue Logdatei gestartet am " + dateStr + " ===");
    return 1; // Erfolg
  }
}

// Hilfsfunktion: 04 statt 4
String addLeadingZero(int number) {
  if (number < 10) return "0" + String(number);
  return String(number);
}
