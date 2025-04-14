#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3231.h>
#include <Wire.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 4
DS3231 rtc(SDA, SCL);
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

int deviceCount = 0;
float tempC;

void setup(void)
{
  sensors.begin();  // Start up the library
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices. at:");
  Serial.print(rtc.getDateStr());
  Serial.print(rtc.getTimeStr());
  Serial.println("");
}

void loop(void)
{ 
  // Send command to all the sensors for temperature conversion
  sensors.requestTemperatures(); 
  Serial.print("Temp at ");
  Serial.print(rtc.getDateStr());
  Serial.print(", ");
  Serial.print(rtc.getTimeStr());
  Serial.print(": ");
  // Display temperature from each sensor
  for (int i = 0;  i < deviceCount;  i++)
  {
    Serial.print(i+1);
    Serial.print(" ");
    tempC = sensors.getTempCByIndex(i);
    Serial.print(tempC);
    Serial.print(" ");
    Serial.print("C ");
   
  }
  
  Serial.println("");
  delay(1000);
}
