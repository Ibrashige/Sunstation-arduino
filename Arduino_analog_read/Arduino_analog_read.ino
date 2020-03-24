// Print battery voltage
// with Voltage Divider (2x 10K resistor)
/*
  Resistors are aligned in series.
  One end goes to Battery - and also to Arduino GND
  The other goes to Battery + and also to Arduino Vin
  The middle (connection between two resistors) goes to Arduino A0
*/

#include <Wire.h>
#define led_pin 13

void setup()
{
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
}

void loop()
{
  printVolts();
}
 
 void printVolts()
{
  int sensorValue = analogRead(A0); //read the A0 pin value
  float voltage = sensorValue * (5.00 / 1023.00) * 2; //convert the value to a true voltage.
  Serial.print("voltage = ");
  Serial.print(voltage); 
  Serial.println("V");
  
  if (voltage < 3.50) //set the voltage considered low battery here
  {
    digitalWrite(led_pin, HIGH);
  }

  delay(5000);
}
