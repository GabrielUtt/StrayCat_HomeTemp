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

int CS_PIN = 10; // Bei Uno = 10, Mega = 53

String currentDateString = "";  // Zum Überwachen, ob sich das Datum ändert
unsigned long lastWriteTime = 0;
const unsigned long writeInterval = 10000; // z.B. alle 60 Sek. schreiben

int deviceCount = 0;
float tempC;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  sensors.begin();

  if (!SD.begin()) {
    Serial.println("SD-Karten-Fehler!");
    while (1);
  }
  Serial.println("SD-Karte bereit.");

  // Initialisieren mit aktuellem Datum
  currentDateString = getDateString();
  openLogFile(currentDateString);
}

void loop() {
  unsigned long now = millis();

  // Alle X Sekunden/Millisekunden Daten loggen
  if (now - lastWriteTime >= writeInterval) {
    lastWriteTime = now;

    // Prüfe, ob sich das Datum geändert hat
    String today = getDateString();
    if (today != currentDateString) {
      Serial.println("Neuer Tag erkannt! Neue Datei wird erstellt.");
      currentDateString = today;
      openLogFile(currentDateString);
    }

    // Schreibe in die aktuelle Datei
    if (myFile) {
      String timeStr = rtc.getTimeStr();
      myFile.print(timeStr);

      for (int i = 0;  i < deviceCount;  i++)
  {
    Serial.print(i+1);
    Serial.print(", ");
    tempC = sensors.getTempCByIndex(i);
    Serial.print(tempC);
    Serial.print(", ");
   
  }
      myFile.println("");
      myFile.flush(); // WICHTIG: Daten sicher speichern
      Serial.println("Eintrag gespeichert: " + timeStr);
    }
  }
}

// Holt aktuelles Datum im Format YYYY-MM-DD
String getDateString() {
  String rawDate = rtc.getDateStr(); // z. B. "13/04/2025"
  
  // Aufteilen in Tag, Monat, Jahr
  int day = rawDate.substring(0, 2).toInt();
  int month = rawDate.substring(3, 5).toInt();
  int year = rawDate.substring(6, 10).toInt();

  // Formatieren zu "YYYY-MM-DD"
  return String(year) + "-" + addLeadingZero(month) + "-" + addLeadingZero(day);
}


// Öffnet Datei für aktuellen Tag
void openLogFile(String dateStr) {
  if (myFile) myFile.close(); // Alte Datei schließen

  String filename = dateStr + ".txt";
  Serial.println("Öffne Datei: " + filename);
  myFile = SD.open(filename.c_str(), FILE_WRITE);

  

  if (myFile) {
    Serial.println("Fehler beim Öffnen der Datei!");
  } else {
    myFile.println("=== Neue Logdatei gestartet am " + dateStr + " ===");
  }
}

// Hilfsfunktion: 04 statt 4
String addLeadingZero(int number) {
  if (number < 10) return "0" + String(number);
  return String(number);
}
