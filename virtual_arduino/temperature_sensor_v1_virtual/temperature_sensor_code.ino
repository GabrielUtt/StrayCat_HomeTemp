// C++ code
//
int temp = A0;
float temp_celsius;
float Voltage;
float analog_reading;
float temp_far; // Fahrenheit


void setup()
{
  pinMode(A0, INPUT); // set temp sensor as input
  Serial.begin(9600);
}

void loop()
{
  analog_reading = analogRead(A0); // read analog reading
  Serial.print("Analog reading:"); 
  Serial.println(analog_reading);
  
  // convertin analog reading to voltage
  Voltage = analog_reading * 5 / 1024;
  // we divide by 1024 as the analog reading from the data pin
  // is between 0 and 1023
  // 5 because 5Volts
  
  Serial.print("Voltage reading:"); // display Voltage
  Serial.println(Voltage);
  
  // reading in celsius:
  // converting from 10 mv per degree with 0.5V offset
  
  temp_celsius = 100 * (Voltage - 0.5);
  Serial.println("Celsius reading:" + String(temp_celsius) + (" C"));
  temp_far = (temp_celsius * 9/5) + 32;
  Serial.println("Fahrenheit reading:" + String(temp_far) + (" F\n"));
  delay(3000); // 3 seconds before repeating the loop
}